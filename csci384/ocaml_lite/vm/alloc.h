/**
 * @file   alloc.h
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  Memory allocator for the OVM.
 *
 * This header defines the interface to the OVM memory allocator and garbage
 * collector. To use the memory allocator, a program must call
 * ovm_allocator_init() first. Then any number of calls may be made to
 * ovm_alloc(). Most of the time, it is not necessary to call collect_garbage()
 * except once at the end of the program, but it is provided to allow manual 
 * collection when necessary. A program should finalize the allocator by 
 * calling ovm_allocator_free().
 */

#pragma once

#include <stdlib.h>
#include "stack.h"

/** @brief The type of a heap-allocated object.
 *
 * A string is a block of memory with no pointers to other heap-allocated 
 * objects. A closures has a header followed by pointers to potentially 
 * heap-allocated objects. An environment is an array of potentially 
 * heap-allocated objects.
 */
typedef enum { STRING, CLOSURE, ENV } tag_t;

/** @brief Allocate space for an OVM object.
 *
 * This function allocates space for a new OVM object, potentially collecting 
 * garbage if there is not enough space for the allocation. The three local 
 * object parameters allow local variables to be marked as "live" for the 
 * garbage collector even if those variables are not reachable from the stack 
 * or the current environment of the OVM.
 *
 * @param bytes  The number of bytes to allocate.
 * @param type   The type of the object being allocated.
 * @param local1 An object which should not be deleted.
 * @param local2 An object which should not be deleted.
 * @param local3 An object which should not be deleted.
 * @return A pointer to the newly allocated memory.
 *
 * @exception Out-of-memory error can be caused if there is not enough space 
 * left on the heap to allocate the requested number of bytes after garbage 
 * collection.
 *
 * @warning No two of local1, local2, and local3 should point to the same 
 * heap-allocated object.
 *
 * @donotmerge
 *
 * @warning The garbage collector only checks the OVM stack, current 
 * environment, and local parameters for live values. The result of any call to 
 * ovm_alloc needs to be reachable from one of those locations before the next 
 * call to ovm_alloc() or it may be deleted by the garbage collector.
 */
void* ovm_alloc(size_t bytes, tag_t type,
                object* local1, object* local2, object* local3);

/** @brief Delete unused objects.
 *
 * This function will delete unused objects in the generation 1 heap. Note that 
 * it does not affect the generation 0 heap.
 *
 * @param gc0_objs A set of objects that are currently live from generation 0 
 * garbage collection. This parameter may be NULL if there are no such objects.
 *
 * @param local1 An obhect which should not be deleted.
 * @param local2 An obhect which should not be deleted.
 * @param local3 An obhect which should not be deleted.
 */
void collect_garbage(stack* gc0_objs,
                     object local1, object local2, object local3);

/// @brief Initialize the allocator.
void ovm_allocator_init();

/// @brief Free all remaining memory and delete the allocator.
void ovm_allocator_free();
