/**
 * @file   readfile.c
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  Implementation of functions for reading bytecode files.
 */

#include "readfile.h"

size_t get_file_size(FILE* fp) {

#ifdef UNIX
    struct stat st;
    fstat(fileno(fp), &st);
    return st.st_size;

#elif defined WINDOWS
    return _filelength(_fileno(fp));

#else
    fprintf(stderr, "Can't determine file size on this platform.\n");
    exit(EXIT_FAILURE);

#endif

}

Bytes read_bytecode_file(char* filename, size_t** fn_ptrs, size_t* num_fns) {
    FILE* source = fopen(filename, "rb");
    if (source == NULL) {
        fprintf(stderr, "Failed to open bytecode file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    size_t file_size = get_file_size(source);
    uint8_t* bc = malloc(file_size);
    size_t bytes_read = fread(bc, 1, file_size, source);
    if (bytes_read < file_size) {
        fprintf(stderr, "Error reading bytecode file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fclose(source);
    Bytes code;
    code.array = bc;
    code.length = file_size;
    // Check the magic number.
    if (code.length < 8 || code.array[0] != 0x52 || code.array[1] != 0x65 ||
        code.array[2] != 0x65 || code.array[3] != 0x64 || code.array[4] != 0x50 ||
        code.array[5] != 0x4C || code.array[6] != 0x44 || code.array[7] != 0x49) {
        fprintf(stderr, "Bad magic number in OVM bytecode file\n");
        exit(EXIT_FAILURE);
    }
    // Make sure the file contains at least the number of functions and one
    // function pointer.
    if (code.length < 24) {
        fprintf(stderr, "Ill-formed OVM bytecode file\n");
        exit(EXIT_FAILURE);
    }
    // Read the number of functions.
    *num_fns = 0;
    for (int i = 8; i < 16; i++) {
        *num_fns <<= 8;
        *num_fns |= code.array[i];
    }
    // Read all function pointers.
    *fn_ptrs = malloc(*num_fns * sizeof(size_t));
    for (size_t i = 0; i < *num_fns; i++) {
        (*fn_ptrs)[i] = 0;
        for (int j = 0; j < 8; j++) {
            (*fn_ptrs)[i] <<= 8;
            (*fn_ptrs)[i] |= code.array[16 + 8*i + j];
        }
    }
    return code;
}
