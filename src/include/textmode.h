// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/textmode.h
// = Purpose:  Header for the primary VGA textmode driver
// ========================================================================== //

#ifndef TEXTMODE_H_INCLUDED
#define TEXTMODE_H_INCLUDED

#include <types.h>
#include <colors.h>

/** Structure that defines the two bytes in a word in VRAM. They contain the
 *   attribute field (essentially color, but also a blink bit (?)) and the 
 *   ASCII value that is printed to screen. Depending on the motherboard, values
 *   above 127 can render in various character sets.
 */
typedef struct __CHAR_T
{
     byte   character;
     byte   color;
     
} packed vgachar_t;


/** VGA Parameters structure - A word of data that contains an 8-bit field for
 *   switching settings, and then a character that represents the blank char.
 */
typedef struct __VGA_PARAM
{
     byte scroll :1;     // Enable/disable screen scrolling
     byte cursor :1;     // Enable/disable text cursor
     byte topbar :1;     // Enable/disable static top bar
     byte tabs   :3;     // Width of tab space (1-8 spaces)
     byte p7     :1;
     byte p8     :1;
     
     byte blank;    
      
} packed vgaparam_t;

enum __VGA_PARAM_NAMES
{
     _vgascroll, _vgacursor, _vgatopbar, _vgatabs,
     __p3, __p4, __p5, __p6, __p7,
};

int vga_param(byte, byte);   // Sets vga driver parameters

int vga_set_bg(color_t);     // Sets the background color at the cursor
int vga_set_fg(color_t);     // Sets the foreground color at the cursor
int vga_set_color(color_t);  // Sets a fg/bg pair using a palette   
int vga_clear(void);         // Overwrites the screen with blanks


int vga_print_char(char);     // Prints a single character at the cursor, 
int vga_increment(void);      // Increments the cursor
                              
int vga_sync_frame(void);     // Updates VRAM with the framebuffer
int vga_movexy(int,int);    // Moves the cursor to x, y
#endif