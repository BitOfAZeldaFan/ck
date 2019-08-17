// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = gdt code
// ========================================================================== //

#include <gdt.h>
#include <mman.h>
#include <stdlib.h>

volatile raw_gdt_t *const GDTPOINTER = (raw_gdt_t*) __gdt_base_addr;
volatile uint16_t  *const GDTSIZE = (uint16_t*) __gdt_size;



int gdt_init()
{
     extern int reload_gdt(uint32_t);
     /* GDT descriptors for flat 4gb */
     gdt_entry_t null_desc = { 0,0,0,0 };
     gdt_entry_t code_desc = { 0,0xffffffff, 0x9a, 0x04 };
     gdt_entry_t data_desc = { 0,0xffffffff, 0x92, 0x04 };
     gdt_add_entry(null_desc);
     *GDTSIZE = 0;
     
     //reload_gdt(__gdt_base_addr);
     __asm__("lgdt 0x1000");
     return 0;
}

/**  @Function      gdt_add_entry()
 *   @Arguments:    GDT entry structure with base, limit, access, and flags
 *   
 *   Takes a generic GDT entry structure with human-readable values and
 *    translates it into the proper bitfields for the complex gdt structure.
 *    The function creates a temporary gdt entry, writes to that, and finally
 *    appends the existing GDT in memory with the additional data.
 */
int gdt_add_entry(gdt_entry_t entry)
{
     raw_gdt_t new_entry;
     /* Translate the entry's base address */
     new_entry.base_l = (entry.base & 0x0000ffff);        // Low 16 bits
     new_entry.base_m = (entry.base & 0x00ff0000) >> 16;  // Middle 8 bits
     new_entry.base_h = (entry.base & 0xff000000) >> 24;  // High 8 bits
     
     /* Translate the entry's address limit */
     new_entry.limit_l = (entry.limit & 0x0000ffff);      // Low 16 bits
     new_entry.limit_h = (entry.limit & 0x000f0000);      // High 4 bits
     
     /* Translate everything else */
     new_entry.access = entry.access;
     new_entry.flags = entry.flags & 0x0f;                // Low 4 bits
     
     /* Increment the size of the gdt */
     *GDTSIZE = *GDTSIZE + 8;
     
     /* Clone the new entry into memory */
     GDTPOINTER[*GDTSIZE/8] = new_entry;     // Remember that the size is the
                                             //  length of the GDT in bytes - 1!
     
     
     
     
     return 0;
}