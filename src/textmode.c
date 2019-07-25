// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     textmode.h
// = Purpose:  Implements vga textmode screenprinting
// ========================================================================== //

#include <textmode.h>
#include <stdio.h>
#include <mmap.h>

// Standard text mode parameters
#define VGA_SIZE    2000 // Number of characters in 80 x 25 text mode
#define MAX_COLS    80   // Number of columns in text mode
#define MAX_ROWS    25   // Number of rows in text mode

// Static memory addresses for VGA functions
// @todo: need to keep better track of these!
volatile vga_char_t *VRAM = (vga_char_t*) __vram_addr;
volatile vga_char_t *VGABUFFER = (vga_char_t*) __vga_buffer_addr;
volatile color_t    *VGACOLOR = (color_t*)__vga_color_addr;
volatile word       *VGACURSOR = (word*)__vga_cursor_addr;
volatile vga_param_t*VGAPARAM = (vga_param_t*)__vga_param_addr;

// ========================================================================== // 
//  Private functions Declaration
// ========================================================================== //
int vga_scroll(void);         // Scrolls the screen by one row
int vga_return(void);         // Implements combined newline (\n) and CR (\r)
int vga_home(void);           // Homes cursor to column 0
int vga_newline(void);        // Increases row by 1
int vga_tab(void);            // Prints a tab character

// ========================================================================== // 
//  Public functions Implementation
// ========================================================================== //

/**  @Function:     vga_set_bg() and vga_set_fg()
 *   @Arguments:    Four-bit hex nyble for the attribute byte
 *
 *   Sets the background/foreground color at the current cursor. Any
 *    subsequent text added to the screen will continue to use this
 *    new color until changed. It is probably better to use palette
 *    pairs unless setting specific fg/bg colors is needed.
 */
int vga_set_bg(color_t color)
{
     // Sets the high 4 bits to the color provided
     *VGACOLOR = (*VGACOLOR & 0x0F) | (color << 4);
     return 0;
}

int vga_set_fg(color_t color)
{
     // Sets the lower 4 bits to the color provided
     *VGACOLOR = (*VGACOLOR & 0xF0) | (color & 0x0F);     
     return 0;
}

/**  @Function:     vga_set_color()
 *   @Arguments:    color_t byte as a 'palette' of sorts.
 *
 *   Sets the combined foreground and background color. There is an enum of
 *    predefined palettes starting with "C_" defined in <colors.h> that can be
 *    used, otherwise any 2-digit hex will make fancy colors. For now, colors
 *    are limited to the 16-color VGA textmode palette. 
 */
int vga_set_color(color_t color)
{
     *VGACOLOR = color;
     return 0;
}

/**  @Function:     vga_print_char()
 *   @Arguments:    ASCII character
 *
 *   Prints a single character to the frame buffer. Please note, this does not 
 *    write directly to vram. Characters are printed to the frame buffer which 
 *    needs to be copied to vram manually by calling vga_sync_frame()
 */
int vga_print_char(char c)
{
     switch(c)
     {
          case '\n': vga_return(); break;
          case '\t': vga_tab(); break;
          default: 
               VGABUFFER[*VGACURSOR].character = c;
               VGABUFFER[*VGACURSOR].color = *VGACOLOR;
          break;
     }  
          
     
     return 0;
}

/**  @Function:     vga_increment()
 *   @Returns:      0 normally, 2 if scrolling is disabled
 *
 *   Increments the cursor manually. If scrolling is turned on, scroll the screen.
 *    If it's not, then keep sticking characters at the end of the screen.
 */
int vga_increment(void)
{
     if(++*VGACURSOR > VGA_SIZE) {
          return vga_scroll();
     }
     return 0;
}

/**  @Function:     vga_sync_frame()
 *
 *   Writes the framebuffer defined in VGABUFFER to vram. The actual buffer is 
 *    not changed.
 */
int vga_sync_frame(void)
{
     for(int i=0;i<VGA_SIZE;i++){
          VRAM[i] = VGABUFFER[i];
     }
     return 0;
}



/**  @Function: vga_clear()
 *
 *   Overwrites the screen buffer with spaces to blank it out. It will
 *    continue to use the existing palette, and place the cursor to
 *    the top of the screen.
 */
int vga_clear(void)
{
     *VGACURSOR = 0;
     for(int i=0;i<VGA_SIZE;i++) {
          vga_print_char(VGAPARAM->blank); 
     }
     *VGACURSOR = 0;
     return 0;
}

/**  @Function:     vga_movexy()
 *   @Arguments:    byte x (cols) and byte y (rows)
 *   @Returns:      0 if there are no errors
 *                  1 if cols or rows is out of bounds.
 *
 *   Moves the cursor to position (x,y) relative to top-left of screen.
 *    If one of the arguments are not in bounds, this function does not change
 *    cursor at all, and returns with an error. Also note, location counting
 *    starts at 0 like normal.
 */
int vga_movexy(int x, int y)
{
     if(x<MAX_COLS && y<MAX_ROWS) {
          *VGACURSOR = (y*MAX_COLS)+x;
     }
     else {
          return 1;
     }
     return 0;
}

/**  @Function:     vga_param()
 *   @Arguments:    2 bytes - a bitfield and ASCII value
 *   @Returns:      0 if normal termination
 *                  1 if parameters don't make sense
 *   
 *   Sets the VGA driver parameters. First byte is the parameter bitfield, 
 *    second byte is the blank character used in scroll() and stuff. Most of
 *    the bitfields are not used yet, but they might be implemented. If one
 *    parameters don't make sense, the function will return with an error.
 */
int vga_param(byte field, byte blank)
{
     VGAPARAM->field = field;
     VGAPARAM->blank = blank;
     return 0;
}

// ========================================================================== // 
//  Private functions Implementation
// ========================================================================== //

/**  @Function:     vga_scroll()
 *   @Returns:      0 if normal execution
 *                  2 if scrolling is disabled
 *
 *   Implements textmode scrolling by copying the second row onto the first,
 *    then the third on to the second, etc. At the end, it overwrites the last
 *    row with blank characters.
 *   If scrolling is disabled, function does nothing and returns 2.
 */  
int vga_scroll(void)
{    
     // Check is scrolling is enabled
     if(VGAPARAM->field & 0b01000000) {
          // Copy row by row
          for(int i=80;i<VGA_SIZE;i++){
               VGABUFFER[i-80] = VGABUFFER[i];
          }
          // Clear the last row
          for(int i=VGA_SIZE-MAX_COLS;i<VGA_SIZE;i++) {
               VGABUFFER[i].character = VGAPARAM->blank;
               VGABUFFER[i].color = *VGACOLOR;
          }
     } else {
          return 2;
     }
     
     *VGACURSOR = VGA_SIZE-80;
     return 0;
}

/**  @Function: vga_return(), vga_newline(), vga_home()
 *   
 *   Implements a three simple screen manipulation functions- vga_return() is
 *    a combined newline and carraige return, while vga_newline() and vga_home()
 *    send the cursor down one line or to the beginning of the line, respectivly.
 *    Use these for implementing '\n' and '\r' escape characters. 
 */
int vga_newline(void)
{
     // Subtracting 1 because print_char auto increments cursor (bad idea?)
     *VGACURSOR = *VGACURSOR + MAX_COLS - 1;
     // Check if screen needs to scroll
     if(*VGACURSOR > VGA_SIZE) {
          vga_scroll();
     }
     return 0;
}
int vga_home(void)
{
     *VGACURSOR = *VGACURSOR - *VGACURSOR % MAX_COLS;
     return 0;
}
int vga_return(void)
{
     vga_home();
     vga_newline();
     return 0;
}
int vga_tab(void)
{
     for(int i=0;i<VGA_TABWIDTH;i++) {
          vga_print_char(VGAPARAM->blank);
          vga_increment();
     }
     return 0;
}
