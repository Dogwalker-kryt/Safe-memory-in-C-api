#ifndef SAFENEN_H
#define SAFEMEM_H

#include <stddef.h>

//  allocates memory with error checking (no NULL return, program aborts on error)
void* sm_malloc(size_t size);

// reallocates memory with error checking
void* sm_realloc(void* ptr, size_t new_size);

// allocates memory and sets it to 0
void* sm_calloc(size_t num, size_t size);

// frees memory and removes it from internal tracking
void sm_free(void* ptr);

// Displays all memory blocks that have not yet been released (leak detection)
void sm_report_leaks(void);





#endif