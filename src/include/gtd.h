// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/gdt.h
// = Purpose:  Typedef for global descriptor table
// ========================================================================== //

typedef struct __GDT_T
{
     uint16_t  limit_l;
     uint16_t  base_l;
     uint8_t   base_h;
     uint8_t   access;
     uint8_t   limit:4;
     uint8_t   flags:4;
     uint8_t   base_h;
     
} packed gdt_t;

void gtd_add_entry(word, word, word);
