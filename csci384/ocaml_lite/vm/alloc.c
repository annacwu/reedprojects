/**
 * @file   alloc.c
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  The implementation of the OVM memory allocator.
 *
 * This file implements the allocator and garbage collector for the OVM. The 
 * OVM uses a generational garbage collector with two generations, referred to 
 * as generation 0 and generation 1. Each generation has it's own heap with the 
 * generation 1 heap being much larger than the generation 0 heap. Small 
 * objects (at most SMALL_SIZE_LIMIT bytes) are all initially created on the 
 * generation 0 heap. Once we run out of space in the generation 0 heap, any 
 * live objects are copied from generation 0 to generation 1 and the generation 
 * 0 heap is reused for new objects. When we run out of space in the generation
 * 1 heap, it is garbage collected.
 *
 * The idea behind this strategy is that most objects in OCaml-lite code are
 * small and short-lived. Such objects will be created on the generation 0 heap 
 * and (often) deleted from the generation 0 heap without ever moving into 
 * generation 1. Any objects that do make it to generation 1 are likely to be 
 * long-lived. Therefore, we perform frequent collections on a small heap 
 * (generation 0) and relativley rare collections on the larger generation 1 
 * heap. In addition, the size limit of generation 0 objects allows allocation 
 * and garbage collection to be much faster.
 *
 * Garbage collection within each heap is done by following pointers from a set 
 * of _root_ objects. In the OVM, the root objects are the OVM stack, the 
 * current environment, and a few local variables which the garbage collection 
 * function takes as parameters. For each value in these roots, we check 
 * whether the value is a pointer and if it is, we mark the object as "live". 
 * Then we recursively follow any pointers contained within that object to find 
 * further live values.
 *
 * In generation 0 collection, live values are copied into generation 1 and all 
 * pointers that point to those objects are updated. In generation 1, all 
 * objects that have not been marked as live are unreachable, so they are 
 * freed. In order to track unreachable objects, the allocator for generation 1 
 * keeps track of a list of all allocated objects, called the _used list_.
 *
 * For the marking phase, the garbage collector needs to be able to distinguish 
 * between pointers and non-pointer values. We handle this by taking advantage 
 * of the fact that pointer alignment rules will require all pointers to end 
 * with a zero. Therefore, we represent non-pointer values in the most 
 * significant 63 bits of each word and insert a 1 in the least significant 
 * bit. The garbage collector can safely assume any value ending with a 1 bit 
 * is not a pointer and any value ending with a 0 bit is.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "alloc.h"

/// @brief The maximum amount of generation 1 memory to allocate.
#ifdef DEBUG
const size_t MAX_HEAP_SIZE = 0x1000;   // Maximum amount of memory for the heap.
#else
const size_t MAX_HEAP_SIZE = 0x40000000; // Maximum amount of memory for the heap.
#endif

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/// @brief The size of the generation 0 heap.
#ifdef DEBUG
const size_t GEN0_SIZE = 1024;
#else
const size_t GEN0_SIZE = 256 * 4096;  // 256 pages, 1 MB
#endif

const size_t SMALL_SIZE_BITS = 6;
/// @brief the size of objects on the generation 0 heap.
const size_t SMALL_SIZE_LIMIT = 1 << SMALL_SIZE_BITS;

// The OVM global state, see machine_state.c
extern stack* st;
extern object* current_env;
extern size_t current_env_size;

/** @brief The size of an object header.
 *
 * A header requires at least 8 bytes to track the information needed for the 
 * garbage collector. It may be larger than 8 bytes to ensure that the address 
 * HEADER_SIZE bytes after any allocated pointer is properly aligned for any 
 * heap allocated objects.
 */
static const size_t HEADER_SIZE = MAX(_Alignof(closure), 8);

/// @brief The total amount of memory currently allocated in generation 1.
static size_t total_allocated = 0;

/// @brief A node in a list of allocated blocks.
struct alloc_list_t {
    uint8_t* ptr;
    struct alloc_list_t* next;
};

/// @brief A list tracking all allocated blocks in generation 1.
struct alloc_list_t* used_list = NULL;

/// @brief A pointer to the beginning of the generation 0 heap.
uint8_t* gen0_heap;
/// @brief The offset of the next free byte in the generation 0 heap.
size_t next_gen0_object = 0;

struct alloc_list_t* gen1_roots = NULL;

static inline int reverse_pointer(uint8_t** parent, uint8_t** ptr) {
#ifdef DEBUG
    printf("Re-reversing: parent=%p (@%p), ptr=%p (@%p)\n", *parent, parent, *ptr, ptr);
#endif
    uint8_t* tmp = *ptr;
    *ptr = *parent;
    if (*ptr == NULL) {
        return 1;
    }
    size_t i = (*ptr)[2];
    (*ptr)[2]++;
    object o;
    OBJ_ENV(o) = (object*) (*parent + HEADER_SIZE);
#ifdef DEBUG
    printf("Parent pointer stored at index %ld in object %p\n", i, OBJ_ENV(o));
#endif
    *parent = (uint8_t*) OBJ_CLOS(OBJ_ENV(o)[i]);
    OBJ_CLOS(OBJ_ENV(o)[i]) = (closure*) (tmp + HEADER_SIZE);
#ifdef DEBUG
    printf("After re-reversing: parent=%p, ptr=%p\n", *parent, *ptr);
#endif
    return 0;
}

/** @brief Mark an object as reachable for the garbage collector.
 *
 * @param root The object to mark.
 */
void mark(object root) {
    uint8_t* parent = NULL;
    uint8_t* ptr = (uint8_t*) OBJ_STR(root);
    ptr -= HEADER_SIZE;
    while (ptr != NULL) {
        object o;
        OBJ_STR(o) = (char*) (ptr + HEADER_SIZE);
        if (ptr[1]) {
            int done = 0;
            if (ptr[0] == STRING) {
                done = 1;
            } else if (ptr[0] == CLOSURE) {
                object o;
                OBJ_CLOS(o) = (closure*) (ptr + HEADER_SIZE);
                size_t length = o.f[0].header.env_size;
                if (ptr[2] > length) {
                    done = 1;
                }
            } else {
                size_t length = ((uint32_t*) ptr)[1] / sizeof(object);
                if (ptr[2] >= length) {
                    done = 1;
                }
            }
            // Already marked, re-reverse pointer
            if (done) {
                done = reverse_pointer(&parent, &ptr);
                if (done) {
                    return;
                }
                continue;
            }
        }
        ptr[1] = 1;
#ifdef DEBUG
        printf("Marked object %p\n", ptr + HEADER_SIZE);
#endif
        if (ptr[0] == STRING) {
            int done = reverse_pointer(&parent, &ptr);
            if (done) {
                return;
            }
            continue;
        } else if (ptr[0] == CLOSURE) {
            if (cl_env(OBJ_CLOS(o)) != NULL) {
                size_t length = o.f[0].header.env_size;
                size_t i = ptr[2];
                if (i == 0) {
                    i++;
                    ptr[2] = i;
                }
                while (i <= length && ((OBJ_INT(OBJ_CLOS(o)[i].obj) & 1) ||
                                       OBJ_CLOS(OBJ_CLOS(o)[i].obj) ==
                                       (closure*) (ptr + HEADER_SIZE))) {
                    // Closures will have a reference to themselves which can
                    // be skipped.
                    i++;
                    ptr[2] = i;
                }
                if (i > length) {
                    int done = reverse_pointer(&parent, &ptr);
                    if (done) {
                        return;
                    }
                    continue;
                }
#ifdef DEBUG
                printf("Marking environment from closure: %p (from %p[%d])\n",
                       OBJ_STR(OBJ_CLOS(o)[i].obj), OBJ_CLOS(o), i);
#endif
                uint8_t* tmp = (uint8_t*) OBJ_CLOS(OBJ_CLOS(o)[i].obj) - HEADER_SIZE;
                OBJ_CLOS(OBJ_CLOS(o)[i].obj) = (closure*) parent;
                parent = ptr;
#ifdef DEBUG
                printf("New parent pointer: %p\n", parent);
#endif
                ptr = tmp;
#ifdef DEBUG
                printf("New main pointer: %p\n", ptr);
#endif
            }
        } else {
            size_t length = ((uint32_t*) ptr)[1] / sizeof(object);
            size_t i = ptr[2];
            while (i < length && (OBJ_INT(OBJ_ENV(o)[i]) & 1)) {
                i++;
                ptr[2] = i;
            }
            if (i >= length) {
                int done = reverse_pointer(&parent, &ptr);
                if (done) {
                    return;
                }
                continue;
            }
#ifdef DEBUG
            printf("Marking object %p from environment %p at %ld\n",
                   OBJ_ENV(OBJ_ENV(o)[i]), OBJ_ENV(o), i);
#endif
            uint8_t* tmp = (uint8_t*) OBJ_CLOS(OBJ_CLOS(o)[i].obj) - HEADER_SIZE;
            OBJ_CLOS(OBJ_ENV(o)[i]) = (closure*) parent;
            parent = ptr;
            ptr = tmp;
        }
    }
}

void collect_garbage(stack* gc0_objs, object local1, object local2, object local3) {
#ifdef DEBUG
    printf("\nCollecting garbage...\n");
#endif
    // Mark live values from the generation 0 object stack.
    if (gc0_objs != NULL) {
        for (size_t i = 0; i < gc0_objs->size; i++) {
            if (!(OBJ_INT(gc0_objs->values[i]) & 1)) {
                // This value is not an integer, so it must be a pointer--mark it.
#ifdef DEBUG
                printf("Marking object %p from the gc0 stack\n", OBJ_ENV(gc0_objs->values[i]));
#endif
                mark(gc0_objs->values[i]);
            }
        }
    }
    // Mark live values from the OVM stack.
    for (size_t i = 0; i < st->size; i++) {
        if (!(OBJ_INT(st->values[i]) & 1)) {
            // This value is not an integer, so it must be a pointer--mark it.
#ifdef DEBUG
            printf("Marking object %p from the stack at %ld\n", OBJ_ENV(st->values[i]), i);
#endif
            mark(st->values[i]);
        }
    }
    // Mark the current environment as live.
    object env;
    env.e = current_env;
#ifdef DEBUG
    printf("Marking the current environment: %p\n", current_env);
#endif
    mark(env);
    // Mark each local value.
    if (!(OBJ_INT(local1) & 1)) {
        mark(local1);
    }
    if (!(OBJ_INT(local2) & 1)) {
        mark(local2);
    }
    if (!(OBJ_INT(local3) & 1)) {
        mark(local3);
    }

    // Sweep phase -- delete all unmarked memory blocks.
    struct alloc_list_t* node = used_list;
    struct alloc_list_t* prev = NULL;
    while (node != NULL) {    // Loop over the used list
        struct alloc_list_t* next = node->next;
        if (!node->ptr[1]) {  // This node is not marked, so it can be freed.
            if (prev == NULL) {
                used_list = next;
            } else {
                prev->next = next;
            }
            // Remove the size of the freed node from the total allocation.
            size_t size = ((uint32_t*) node->ptr)[1];
            total_allocated -= HEADER_SIZE + size;
#ifdef DEBUG
            printf("Freeing %p\n", node->ptr + 8);
#endif
            free(node->ptr);
            free(node);
        } else {
            // If this node was marked, unmark it in preparation for the next
            // garbage collection cycle.
            node->ptr[1] = 0;
            node->ptr[2] = 0;
            prev = node;
        }
        node = next;
    }
    // Unmark all objects on the gen0 heap
    for (uint8_t* ptr = gen0_heap;
         ptr < gen0_heap + next_gen0_object;
         ptr += SMALL_SIZE_LIMIT) {
        ptr[1] = 0;
        ptr[2] = 0;
    }
}

/** @brief Insert metadata into a block header.
 *
 * @param ptr A pointer to the header to modify.
 * @param type The type of object being created.
 * @param bytes The size of the block being allocated.
 */
void initialize_header(uint8_t* ptr, tag_t type, size_t bytes) {
    ptr[0] = type;  // Insert the type of object into the header.
    ptr[1] = 0;     // Ensure the new object is unmarked to begin with.
    ptr[2] = 0;     // Index used during garbage collection.
    ((uint32_t*) ptr)[1] = bytes;   // Record the size of the object.
}

/** @brief Allocate space on the generation 1 heap.
 *
 * @param bytes The amount of space to allocate.
 * @param type The type of objecting being constructed.
 * @param gc0_obs A set of live objects from the generation 0 heap.
 * @param local1 A local object that should not be deleted.
 * @param local2 A local object that should not be deleted.
 * @param local3 A local object that should not be deleted.
 * @return A pointer to the newly-allocated memory.
 *
 * @exception This function can throw an error if there is not enough heap 
 * memory for the new block even after garbage collection.
 */
void* ovm_alloc_gen1(size_t bytes, tag_t type, stack* gc0_objs,
                     object local1, object local2, object local3) {
    // Check if there's enough space
    if (total_allocated + bytes + HEADER_SIZE > MAX_HEAP_SIZE) {
        // If not, collect garbage.
        collect_garbage(gc0_objs, local1, local2, local3);
    }
    // If there still isn't enough space, then the total amount of live heap
    // memory is too large, so we throw an error.
    if (total_allocated + bytes + HEADER_SIZE > MAX_HEAP_SIZE) {
        fprintf(stderr, "Out of heap space\n");
        exit(1);
    }
    // Allocate space for the requested object plus a header.
    uint8_t* ptr = malloc(HEADER_SIZE + bytes);
#ifdef DEBUG
    printf("Allocated %ld bytes at %p (gen1) with type %d\n", bytes, ptr + HEADER_SIZE, type);
#endif
    initialize_header(ptr, type, bytes);
    // Insert the new block into the used list.
    struct alloc_list_t* node = malloc(sizeof(struct alloc_list_t));
    node->ptr = ptr;
    node->next = used_list;
    used_list = node;
    total_allocated += bytes + HEADER_SIZE;
    return ptr + HEADER_SIZE;
}

object check_and_collect_gen0(object, object*, stack*, object, object, object);

/** @brief Move a live generation 0 object to generation 1.
 *
 * @param root The object to move.
 * @param updates A list of already-moved objects from generation 0.
 * @param gc0_objs A stack of live objects from generation 0.
 * @param local1 A local object that should not be deleted.
 * @param local2 A local object that should not be deleted.
 * @param local3 A local object that should not be deleted.
 * @return The new object after root has been moved to generation 1.
 */
object collect_gen0_object(object root, object* updates, stack* gc0_objs,
                           object local1, object local2, object local3) {
    // Retrieve the header of root
    uint8_t *ptr = (uint8_t*) OBJ_ENV(root) - HEADER_SIZE;
    size_t bytes = ((uint32_t*) ptr)[1];
    // Compute the index into the updates array based on the address of  this 
    // block.
    size_t ind = (size_t) (ptr - gen0_heap) >> SMALL_SIZE_BITS;
    // Create a new object for the moved value.
    object new_obj;
    // Allocate space in generation 1 for the moved value.
    OBJ_STR(new_obj) = ovm_alloc_gen1(bytes, ptr[0], gc0_objs, local1, local2, local3);
    updates[ind] = new_obj;
    if (ptr[0] == STRING) {
        // Strings do not point to any other heap-allocated objects, so all we 
        // need to do is copy the string to the new memory block.
#ifdef DEBUG
        printf("Copying string %p to %p (type=%d, size=%lu)\n",
               OBJ_STR(root), OBJ_STR(new_obj), ptr[0], bytes);
#endif
        strcpy(OBJ_STR(new_obj), OBJ_STR(root));
    } else if (ptr[0] == CLOSURE) {
        // For closures, we need to copy the closure header and each object in 
        // the closure environment.
#ifdef DEBUG
        printf("Copying closure %p to %p (type=%d, size=%lu)\n",
               OBJ_CLOS(root), OBJ_CLOS(new_obj), ptr[0], bytes);
#endif
        memcpy(OBJ_CLOS(new_obj), OBJ_CLOS(root), bytes);
        // Make sure new_obj is not freed in the subsequent calls to the 
        // generation 1 allocator.
        stack_push(gc0_objs, new_obj);
        for (size_t i = 0; i < cl_env_size(OBJ_CLOS(new_obj)); i++) {
            set_env(OBJ_CLOS(new_obj),
                    i,
                    check_and_collect_gen0(cl_env(OBJ_CLOS(new_obj))[i], updates,
                                           gc0_objs, local1, local2, local3));
        }
        stack_pop(gc0_objs);
    } else {
        // This is an array of objects, so we need to move each referenced 
        // object
#ifdef DEBUG
        printf("Copying environment object to %p\n", OBJ_ENV(new_obj));
#endif
        size_t length = bytes / sizeof(object);
        // We set bytes to 0xFF initially to avoid attempting to follow 
        // uninitialized pointers. (The only thing that matters is that the 
        // least significant bit is one so that the garbage collector sees 
        // these values as integers rather than pointers.)
        memset(OBJ_ENV(new_obj), 0xFF, bytes);
        stack_push(gc0_objs, new_obj);
        for (size_t i = 0; i < length; i++) {
#ifdef DEBUG
            printf("Env object %d before copying: %p\n", i, OBJ_ENV(root)[i]);
#endif
            OBJ_ENV(new_obj)[i] = check_and_collect_gen0(OBJ_ENV(root)[i], updates, gc0_objs,
                                                         local1, local2, local3);
#ifdef DEBUG
            printf("Env object %d after copying: %p\n", i, OBJ_ENV(new_obj)[i]);
#endif
        }
        stack_pop(gc0_objs);
    }
    return new_obj;
}

/** @brief Move an object from the generation 0 heap to the generation 1 heap.
 *
 * @param root The object to move.
 * @param updates A list of already-moved objects from generation 0.
 * @param gc0_objs A stack of live objects from generation 0.
 * @param local1 A local object that should not be deleted.
 * @param local2 A local object that should not be deleted.
 * @param local3 A local object that should not be deleted.
 * @return The new object after root has been moved to generation 1.
 */
object check_and_collect_gen0(object root, object* updates, stack* gc0_objs,
                              object local1, object local2, object local3) {
    if (OBJ_INT(root) & 1) {
        // In this case the object is not a pointer, so there is no memory to 
        // move.
#ifdef DEBUG
        printf("Skipping %ld because it is an integer\n", root.i);
#endif
        return root;
    }
    uint8_t *ptr = (uint8_t*) OBJ_ENV(root) - HEADER_SIZE;
    if (ptr < gen0_heap || ptr >= gen0_heap + GEN0_SIZE) {
        // This object is not in the gen0 heap, nothing to be done.
#ifdef DEBUG
        printf("Skipping %p because it is not on the gen0 heap\n", OBJ_ENV(root));
#endif
        return root;
    }
    // Get the offset into the generation 0 heap of the current object.
    size_t ind = (size_t) (ptr - gen0_heap) >> SMALL_SIZE_BITS;
    if (OBJ_STR(updates[ind]) != NULL) {
        // This object has already moved.
#ifdef DEBUG
        printf("Object already moved: %p to %p\n", OBJ_ENV(root), updates[ind]);
#endif
        return updates[ind];
    } else {
        // In this case we need to move the object.
#ifdef DEBUG
        printf("Moving %p\n", OBJ_ENV(root));
#endif
        return collect_gen0_object(root, updates, gc0_objs, local1, local2, local3);
    }
}

/** @brief Free all generation 0 memory.
 *
 * This function finds all live values in the generation 0 heap and copies them 
 * to the generation 1 heap, updating all pointers to those objects 
 * accordingly. The generation 0 heap is then free to be reused.
 *
 * @param local1 A local object that should not be deleted.
 * @param local2 A local object that should not be deleted.
 * @param local3 A local object that should not be deleted.
 */
void collect_gen0(object* local1, object* local2, object* local3) {
#ifdef DEBUG
    printf("\nCollecting the generation 0 heap...\n");
#endif
    // Generate an array mapping gen0 objects to their new gen1 addresses.
    object updates[GEN0_SIZE >> SMALL_SIZE_BITS];
    // Make sure all pointers are initially NULL
    memset(updates, 0, (GEN0_SIZE >> SMALL_SIZE_BITS) * sizeof(object));
    // We will need a stack to keep track of objects that are currently live on 
    // the generation 0 heap.
    stack* gc0_objs = stack_create(256);
    for (size_t i = 0; i < st->size; i++) {
        // Move each stack value.
#ifdef DEBUG
        printf("Stack value %ld...\n", i);
#endif
        st->values[i] = check_and_collect_gen0(st->values[i], updates, gc0_objs,
                                               *local1, *local2, *local3);
    }
#ifdef DEBUG
    printf("Current environment...\n");
#endif
    // Move the current environment.
    object obj;
    OBJ_ENV(obj) = current_env;
    current_env = OBJ_ENV(check_and_collect_gen0(obj, updates, gc0_objs,
                                                 *local1, *local2, *local3));
    // Update the local values.
#ifdef DEBUG
    printf("Local 1: %p\n", OBJ_ENV(*local1));
#endif
    *local1 = check_and_collect_gen0(*local1, updates, gc0_objs,
                                     *local1, *local2, *local3);
#ifdef DEBUG
    printf("Local 1 (after): %p\n", OBJ_ENV(*local1));
    printf("Local 2: %p\n", local2->e);
#endif
    *local2 = check_and_collect_gen0(*local2, updates, gc0_objs,
                                     *local1, *local2, *local3);
#ifdef DEBUG
    printf("Local 2 (after): %p\n", OBJ_ENV(*local2));
    printf("Local 3: %p\n", local3->e);
#endif
    *local3 = check_and_collect_gen0(*local3, updates, gc0_objs,
                                     *local1, *local2, *local3);
#ifdef DEBUG
    printf("Local 3 (after): %p\n", OBJ_ENV(*local3));
#endif
    struct alloc_list_t* root = gen1_roots;
    while (root != NULL) {
        uint8_t* ptr = root->ptr - HEADER_SIZE;
        // We need to descend one level into the object graph, otherwise
        // check_and_collect_gen0 will skip this object as being in gen1
        if (ptr[0] == CLOSURE) {
            closure* cl = (closure*) root->ptr;
            for (size_t i = 0; i < cl_env_size(cl); i++) {
                set_env(cl, i,
                        check_and_collect_gen0(cl_env(cl)[i], updates, gc0_objs,
                                               *local1, *local2, *local3));
            }
        } else if (ptr[0] == ENV) {
            object* env = (object*) root->ptr;
            size_t length = ((uint32_t*) ptr)[1];
            for (size_t i = 0; i < length; i++) {
                env[i] = check_and_collect_gen0(env[i], updates, gc0_objs,
                                                *local1, *local2, *local3);
            }
        }
        struct alloc_list_t* next = root->next;
        free(root);
        root = next;
    }
    gen1_roots = NULL;
    stack_delete(gc0_objs);
}

/** @brief Allocate space for an object on the generation 0 heap.
 *
 * Generation 0 uses a simple fixed-size bump allocator. Each object is given 
 * SMALL_SIZE_LIMIT bytes even if the requested size is smaller. This allows 
 * both allocation and collection to be fast because traversing the heap is a 
 * simple loop.
 *
 * @param bytes The amount of memory to allocate.
 * @param type The type of object being created.
 * @param local1 A local object that should not be deleted.
 * @param local2 A local object that should not be deleted.
 * @param local3 A local object that should not be deleted.
 * @return A pointer to the newly allocated memory.
 *
 * @exception This function can throw an error if the generation 0 heap is full 
 * and there is not enough space on the generation 1 heap to copy all live 
 * objects.
 */
void* ovm_alloc_gen0(size_t bytes, tag_t type,
                     object* local1, object* local2, object* local3) {
    if (next_gen0_object >= GEN0_SIZE) {
        collect_gen0(local1, local2, local3);
#ifdef DEBUG
        // We free and re-allocate the heap when debugging because this makes 
        // it easier for valgrind to find memory errors.
        free(gen0_heap);
        gen0_heap = malloc(GEN0_SIZE);
#endif
        next_gen0_object = 0;
    }
    uint8_t* ptr = gen0_heap + next_gen0_object;
#ifdef DEBUG
    printf("Allocated %ld bytes at %p (gen0) with type %d\n",
           bytes, ptr + HEADER_SIZE, type);
#endif
    initialize_header(ptr, type, bytes);
    next_gen0_object += SMALL_SIZE_LIMIT;
    return ptr + HEADER_SIZE;
}

void* ovm_alloc(size_t bytes, tag_t type,
                object* local1, object* local2, object* local3) {
    if (bytes + HEADER_SIZE <= SMALL_SIZE_LIMIT) {
        return ovm_alloc_gen0(bytes, type, local1, local2, local3);
    } else {
        void* ptr = ovm_alloc_gen1(bytes, type, NULL, *local1, *local2, *local3);
        // It could be the case that this new object points to a gen0 object
        // from the gen1 heap, so the gen0 collector needs to be aware of it
        // as a root.
        struct alloc_list_t* node = malloc(sizeof(struct alloc_list_t));
        node->ptr = ptr;
        node->next = gen1_roots;
        gen1_roots = node;
        return ptr;
    }
}

void ovm_allocator_init() {
    gen0_heap = malloc(GEN0_SIZE);
}

void ovm_allocator_free() {
    object fake;
    OBJ_INT(fake) = 1;
    collect_garbage(NULL, fake, fake, fake);
    struct alloc_list_t* node = used_list;
    while (node != NULL) {    // Loop over the used list
        struct alloc_list_t* next = node->next;
        free(node->ptr);
        free(node);
        node = next;
    }
    free(gen0_heap);
}
