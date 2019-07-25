#define CK_BUILD  123456789
#define CK_VERSION 12345
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
     // Set up VGA text mode parameters
     vga_set_color(C_MSDOS);
     // Turn on all options and set blank char to space
     vga_param(0xff, ' ');
     vga_clear();    
     
     
     // Print a nice welcome text
     printf("%s", logo);
     
     vga_movexy(20,0); printf("Coffee Kernel - (Experimental)");
     vga_movexy(20,1); printf("Alpha version 0.%i.%i", CK_VERSION, CK_BUILD);
     int status = vga_movexy(20,4);
     if(!status) {
          printf("(C) 2019 Bryan Webb");
     }
     else {
          printf("Error moving cursor: %i", status);
     }
     
     return 0;
}

/**  Function: kernel_loop
 *   Purpose:  Primary kernel running loop. This is where everything interesting
 *              happens inside the kernel after it initializes shit. 
 *   Returns:  0 if the loop encountered no problems.
               Other return values indicate a different error.
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
          status = kernel_loop();
          if(status == 0) { 
               continue; 
          }
          else {
               vga_set_color(C_ERROR);
               printf("Kernel has encountered an error. Please restart.\n");
               break;
          }
     }
     return 0;
}
