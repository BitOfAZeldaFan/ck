// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/textmode.h
// = Purpose:  Header for the primary VGA textmode driver
// ========================================================================== //

#ifndef TEXTMODE_H_INCLUDED
#define TEXTMODE_H_INCLUDED

#include <types.h>
#include <colors.h>

#define VGA_TABWIDTH     5

/** Enumeration that refers to various VGA parameters to be set by
 *   the vga_param() system call.
 */
enum __VGAPARAM_FIELDS
{
     VGAPARAM_SCROLL = 0b1000000,
     VGAPARAM_CURSOR = 0b0100000,
};

/** Structure that defines the two bytes in a word in VRAM. They contain the
 *   attribute field (essentially color, but also a blink bit (?)) and the 
 *   ASCII value that is printed to screen. Depending on the motherboard, values
 *   above 127 can render in various character sets.
 */
typedef struct __CHAR_T
{
     byte   character;
     byte   color;
} packed vga_char_t;


/** VGA Parameters structure - A word of data that contains an 8-bit field for
 *   switching settings, and then a character that represents the blank char.
 *  Bitfields - [0][1][2][3][4][5][6][7]
 *   0- Text scrolling on/off
 *   1- Cursor on/off (todo: implement!)
 *   2 through 7 not used yet
 */
typedef struct __VGA_PARAM
{
     byte field;
     byte blank;     
} packed vga_param_t;

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