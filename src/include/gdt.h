// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/gdt.h
// = Purpose:  Typedef for global descriptor table
// ========================================================================== //

#ifndef GDT_H_INCLUDED
#define GDT_H_INCLUDED

#include <types.h>

typedef struct __RAW_GDT_T
{
     uint16_t  base_l;
     uint16_t  limit_l;
     uint8_t   base_m;
     uint8_t   access;
     uint8_t   limit_h:4;
     uint8_t   flags:4;
     uint8_t   base_h;
     
} packed raw_gdt_t;

typedef struct __GDT_ENTRY_T
{
     uint32_t  base;
     size_t    limit;
     uint8_t   access;
     uint8_t   flags;
} packed gdt_entry_t;

int gdt_add_entry(gdt_entry_t);
int gdt_init();

extern void reload_gdt(uint32_t);

#endif
