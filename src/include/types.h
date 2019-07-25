// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/types.h
// = Purpose:  Contains various definitions and typedefs. Mostly typedefs.
// ========================================================================== //

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Various random definitions that do shit
#define packed __attribute__((packed))

/* Renaming the unsigned integers into words I like better */
typedef uint8_t     byte;
typedef uint16_t    word;
typedef uint32_t    dword;

#endif