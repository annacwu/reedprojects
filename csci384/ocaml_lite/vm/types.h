/**
 * @file   types.h
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  The types used within the OVM.
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

/// @brief A byte array with length.
typedef struct {
    uint8_t* array;  ///< The array itself.
    size_t length;   ///< The length of the array.
} Bytes;

union closure;

/// @brief An object in the OVM.
typedef union object {
    int64_t i;  ///< An integer object.
    size_t p;   ///< A pointer, only used for pushing the stack pointer.
    char* s;    ///< A string.
    /** @brief A closure object.
     *
     * A closure is represented as an array of closure unions where the first 
     * is the header and the rest represent the environment.
     */
    union closure* f;
    /** @brief An algebraic data type.
     *
     * This is an array of objects where the first represents the tag of the 
     * algebraic data type and the rest are its fields.
     */
    union object* e;
} object;

/// @brief One component of a closure.
typedef union closure {
    object obj;  ///< An element of the closure's environment.
    /// A header for holding closure meta-data.
    struct {
        uint8_t code_ptr;       ///< A pointer to this closure's code.
        uint8_t env_size;       ///< The current size of the environment.
        /// The total size of the environment when fully applied.
        uint8_t expected_size;
    } header;
} closure;

#define OBJ_INT(o) ((o).i)
#define OBJ_SP(o) ((o).p)
#define OBJ_STR(o) ((o).s)
#define OBJ_CLOS(o) ((o).f)
#define OBJ_ENV(o) ((o).e)

/// @brief Get the code pointer from a closure.
static inline uint8_t cl_code_ptr(closure* cl) {
    return cl[0].header.code_ptr;
}

/// @brief Get the current environment size from a closure.
static inline uint8_t cl_env_size(closure* cl) {
    return cl[0].header.env_size;
}

/// @brief Get the total environment size from a closure.
static inline uint8_t cl_expected_size(closure* cl) {
    return cl[0].header.expected_size;
}

/// Get a reference to the environment of a closure.
static inline object* cl_env(closure* cl) {
    return (object*) cl + 1;
}

/// @brief Set the code pointer of a closure.
static inline void set_code_ptr(closure* cl, uint8_t ptr) {
    cl[0].header.code_ptr = ptr;
}

/// @brief Set the current environment size of a closure.
static inline void set_env_size(closure* cl, uint8_t size) {
    cl[0].header.env_size = size;
}

/// @brief Set the total environment size of a closure.
static inline void set_expected_size(closure* cl, uint8_t size) {
    cl[0].header.expected_size = size;
}

/// @brief Set a value in the closure's environment.
static inline void set_env(closure* cl, size_t ind, object val) {
    cl[ind + 1].obj = val;
}
