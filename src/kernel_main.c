#define CK_VERSION  1
#define CK_BUILD    12
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
#include <gdt.h>

static const char logo[]= {
32, 32, 32,177,177,177, 32, 32, 32, 32, 32,177, 32,177,177, 32, 32,177,177,'\t', '|','\n',
32,177,177, 32, 32, 32, 32, 32, 32, 32,177, 32, 32,177,177,32,177,'\t', '|','\n',
32,177,177, 32, 32, 32, 32, 32, 32,177, 32, 32, 32,177,177,177,'\t', '|','\n',
32,177,177, 32, 32, 32, 32, 32,177, 32, 32, 32, 32,177,177,32,177,'\t', '|','\n',
32, 32, 32,177,177,177, 32,177, 32, 32, 32, 32, 32,177,177, 32, 32,177,177,'\t', '|','\0'
};

// ========================================================================== //
//  Initialization Routines - Declarations
// ========================================================================== //
#define local inline static
int mem_init();
int vga_init();
int idt_init();

/**  @Function init_kernel()
 *   @Returns  0 on success
 *
 *   Runs all the initialization routines, prints out diagnostic data,
 *    and gets the kernel ready to enter KCM (Kernel Command Mode)
 */ 
int init_kernel()
{ 
     gdt_init();   
     size_t memory = mem_init();
     vga_init();
       
     printf("%s", logo);           // Print a nice welcome text     
     vga_set_color(0x07);
     vga_movexy(20,0); printf("Coffee Kernel - (Experimental)");
     vga_movexy(20,1); printf("Alpha version 0.%i.%i", CK_VERSION, CK_BUILD);
     vga_movexy(20,2); printf("%i bytes of memory detected", memory);
     
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

/**  @Function kernel_main()
 *   @Returns  0 if kernel exits normally. Various codes otherwise
 *
 *   Kernel entry point. Starts with initializing the kernel, then executing
 *    the primary loop and checking for errors
 */
int kernel_main()
{
     init_kernel();
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

// ========================================================================== //
//  Initialization Routines - Implementations
// ========================================================================== //

int vga_init()
{
     /* Set up the display */
     vga_set_color(0x05);          // Set up VGA text mode parameters     
     vga_param(0xff, ' ');         // Set blank char to space
     vga_param(0x00, 1);           // Turn on scrolling
     vga_param(0x01, 1);           // Turn on cursor
     vga_param(0x02, 0);           // Disable top bar
     vga_param(0x03, 5);           // Set the tab width
     vga_clear();                  // Clear the screen 
     return 0;
}

int mem_init()
{
     /* Clear the first 511 kb of RAM */
     uint8_t *low_memory = (uint8_t*)0x0;    // Clear the first 511 kb of RAM
     for(int i=0; i<511*1024; i++) {
          low_memory[i] = 0;
     }
     
     /* Need to figure out how much RAM there is */
     return 1000;
}

int interrupts_init()
{
     return 0;
}
