#define CK_BUILD  122
#define CK_VERSION 1
// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = Main kernel start point
// ========================================================================== //
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <textmode.h>
#include <types.h>
#include <stdio.h>
#include <string.h>
#include <mman.h>

static const char logo[]= {
32, 32, 32,177,177,177, 32, 32, 32, 32, 32,177, 32,177,177, 32, 32,177,177,'\t', '|','\n',
32,177,177, 32, 32, 32, 32, 32, 32, 32,177, 32, 32,177,177,32,177,'\t', '|','\n',
32,177,177, 32, 32, 32, 32, 32, 32,177, 32, 32, 32,177,177,177,'\t', '|','\n',
32,177,177, 32, 32, 32, 32, 32,177, 32, 32, 32, 32,177,177,32,177,'\t', '|','\n',
32, 32, 32,177,177,177, 32,177, 32, 32, 32, 32, 32,177,177, 32, 32,177,177,'\t', '|','\0'
};

int init()
{     
     /* Handle memory tasks */
     uint8_t *low_memory = (uint8_t*)0x0;    // Clear the first 511 kb of RAM
     for(int i=0; i<511*1024; i++) {
          low_memory[i] = 0;
     }
     
     /* Set up the display */
     vga_set_color(0x05);          // Set up VGA text mode parameters     
     vga_param(0xff, ' ');         // Set blank char to space
     vga_param(0x00, 1);           // Turn on scrolling
     vga_param(0x01, 1);           // Turn on cursor
     vga_param(0x02, 0);           // Disable top bar
     vga_param(0x03, 5);           // Set the tab width
     vga_clear();                  // Clear the screen     
     printf("%s", logo);           // Print a nice welcome text
     
     vga_set_color(0x07);
     vga_movexy(20,0); printf("Coffee Kernel - (Experimental)");
     vga_movexy(20,1); printf("Alpha version 0.%i.%i", CK_VERSION, CK_BUILD);
     vga_movexy(20,2); printf("Size of mtable_t: %i", sizeof(mtable_t));
     
     return 0;
}

/**  @Function kernel_loop()
 *   @Returns  0 if the loop encountered no problems.
               Other return values indicate a different error.
               
 *   Primary kernel running loop. This is where everything interesting
 *    happens inside the kernel after it initializes shit. 
 *   
 */
int kernel_loop()
{
     
     return 0;
}

int kernel_main()
{
     init();
     vga_sync_frame();
     
     word status=0;
     
     while(true)
     {
          status = kernel_loop();  // Continuously execute the kernel loop
          if(status == 0) { 
               continue;           // Keep running if everything is OK
          }
          else {
               vga_set_color(C_ERROR);
               printf("Kernel has encountered an error. Please restart.\n");
               break;
          }
     }
     return 0;
}
