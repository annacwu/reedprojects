/**
 * @file   readfile.h
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  Functions for reading OVM bytecode files.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "types.h"

// Determine the platform. This is needed because the mechanism by which we
// determine the file size depends on the platform.
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/stat.h>
#define UNIX
#elif defined(_WIN32) || defined(WIN32)
#include <io.h>
#define WINDOWS
#endif

/** @brief Find the number of bytes in a file if possible.
 *
 * @param fp The file to find the size of.
 */
size_t get_file_size(FILE* fp);

/** @brief Read an OVM bytecode file.
 *
 * Reads an OVM bytecode file and parses it. This function checks the magic
 * number then reads the number of functions in the file and all of the function
 * pointers. It returns the entire file as a Bytes object and also modifies
 * fn_ptrs and num_fns to contain the pointers to and number of functions
 * defined in the file.
 *
 * @param filename The name of the file to read.
 *
 * @param fn_ptrs A location to store the list of function pointer to. This 
 * should be an empty location, it will be set to the result of a malloc call.
 *
 * @param num_fns A location to store the number of functions in the file.
 *
 * @return The bytes of the file.
 */
Bytes read_bytecode_file(char* filename, size_t** fn_ptrs, size_t* num_fns);
