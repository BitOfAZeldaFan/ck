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

static const char logo[]= {
32, 32, 32,177,177,177, 32, 32, 32, 32, 32,177, 32,177,177, 32, 32,177,177,'\n',
32,177,177, 32, 32, 32, 32, 32, 32, 32,177, 32, 32,177,177,32,177,'\n',
32,177,177, 32, 32, 32, 32, 32, 32,177, 32, 32, 32,177,177,177,'\n',
32,177,177, 32, 32, 32, 32, 32,177, 32, 32, 32, 32,177,177,32,177,'\n',
32, 32, 32,177,177,177, 32,177, 32, 32, 32, 32, 32,177,177, 32, 32,177,177,'\0'
};

int init()
{     
     /* Handle memory tasks */
     uint8_t *low_memory = (uint8_t*)0x0;    // Clear the first 511 kb of RAM
     for(int i=0; i<511*1024; i++) {
          low_memory[i] = 0;
     }
     
     /* Set up the display */
     vga_set_color(0x05);       // Set up VGA text mode parameters     
     vga_param(0xff, ' ');         // Turn on all options and set blank char to space
     vga_clear();                  // Clear the screen     
     printf("%s", logo);           // Print a nice welcome text
     
     vga_set_color(0x07);
     vga_movexy(20,0); printf("Coffee Kernel - (Experimental)");
     vga_movexy(20,1); printf("Alpha version 0.%i.%i", CK_VERSION, CK_BUILD);
     
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
