// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/mmap.h
// = Purpose:  Map of memory - contains defines that map global kernel values
// ========================================================================== //


/** Core kernel memory is intended to stay within the first megabyte 
 *   ("low" memory). Below is a map of what is free or not. When the kernel
 *   uses a variable that needs to be remembered between function calls in that
 *   source file, those globals are saved as volatile const pointers. 
 *   That means the compiler doesn't optimize the value in its algorithms,
 *   and that nothing can change the memory address of the pointed to values.
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
     
     __vga_buffer_addr   = 0x00000020,  // 4000 bytes 
     __vga_color_addr    = 0x00000fc0,  // 1 byte
                         /* 1b free */
     __vga_cursor_addr   = 0x00000fc2,  // 2 bytes
     __vga_param_addr    = 0x00000fc4,  // 2 bytes
     
     
     
};