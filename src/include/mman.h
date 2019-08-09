// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/mman.h
// = Purpose:  Map of memory - contains defines that map global kernel values
// ========================================================================== //


/** Core kernel memory is intended to stay within the first megabyte 
 *   ("low" memory). Below is a map of what is free or not. When the kernel
 *   uses a variable that needs to be remembered between function calls in that
 *   source file, those globals are saved as volatile const pointers. 
 *   That means the compiler doesn't optimize the value in its algorithms,
 *   and that nothing can change the memory address of the pointed to values.
 *
 *   Variables used should be defined as:
 *        volatile type_t *const VARIABLE = (type_t*) 0xAddress
 *    
 *   All addresses up to 0x007FFFF should be free to use. That's 511 kb
 */
enum __KERNEL_GLOBALS
{
     /* Various location-specific addresses */
     __vram_addr         = 0x000b8000,  // 4000 bytes
     __kernel_addr       = 0X00100000,  // Linker loads kernel at 1M       
     
     /* Sequential addresses */
     
     /* 32 bytes of free memory at the start of RAM for things */
     
     /* VGA data */
     __vga_buffer_addr   = 0x00000020,  // 4000 bytes 
     __vga_color_addr    = 0x00000fc0,  // 1 byte
                         /* 1b free */
     __vga_cursor_addr   = 0x00000fc2,  // 2 bytes
     __vga_param_addr    = 0x00000fc4,  // 2 bytes
                         /* 2 bytes buffer */
     
     /* Memory table data */
     __mtable_free_size  = 0x00000fc8,  // size_t 4 bytes
     __mtable_used_size  = 0x00000fcc,  // size_t 4 bytes
     __mtable_mmap_size  = 0x00000fd0,  // size_t 4 bytes
                         /* 36 bytes free */
                         
     __gdt_size          = 0x00001000,  // Starting at 4kb mark. Size is 2 bytes
     __gdt_base_addr     = 0x00001002,  // gdt shouldn't be more than half a kb
                         /* Assume 1kb for GDT (128 entries) */
     __mtable_free       = 0x00200000,  // Third megabyte
     __mtable_used       = 0x00300000,  // Fourth megabyte
     
     
     
};

/**  Data structure that is used for the memory maps. It's an 8-byte struct
 *    that holds one byte of flags, three bytes of size data, and a four byte
 *    base address.
 */
typedef struct __MEMORY_TABLE_T
{
     uint8_t   flags :8;
     size_t    size  :24;
     uint32_t  addr;
     
} packed mtable_t;