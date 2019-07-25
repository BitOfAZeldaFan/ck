#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED

// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/stdlib.h
// = Purpose:  C Standard Library - Collection of essential C tools
// ========================================================================== //

// Defined macros

#define EXIT_FAILURE     1
#define EXIT_SUCCESS     0
#define RAND_MAX         4294967295

// Functions defined in string.c

int atoi(const char*);   // Converts string to int

// Defined in mman.c

void* calloc(size_t, size_t); // Allocates and initializes memory
void* malloc(size_t);         // Allocates memory but doesn't initialize
void free(void*);             // De-allocates memory
void* realloc(void*, size_t); // Re-allocates existing memory

// Defined 



#endif