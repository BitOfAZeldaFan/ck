// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     std/printf.c
// = Purpose:  Implements terminal output as part of the standard library
// ========================================================================== //

#include <stdio.h>
#include <textmode.h>
#include <string.h>

int printf(const char *str, ...)
{
     // Start processing the variable list of arguments
     va_list arguments;
     va_start(arguments,str);
     byte format_mode = 0;
     char *_str; int _int; float _float; 
     
     // Run the loop unti it reaches a terminating character
     for(size_t i=0;i<strlen(str);i++) {
          //Check if the character is a formatting start character
          if(str[i] == '%' && format_mode == 0) {
               //We are entering formatting mode. Set the flag and continue
               format_mode = 1;
               continue;
          } 
          
          if(format_mode == 1) {
               // We are in format mode, so check what to do with the args
               switch(str[i])
               {
                    case '%': 
                         vga_print_char('%');
                         vga_increment();
                         break; 
                         
                    case 'c':
                         _int = va_arg(arguments, int);
                         vga_print_char((char)_int);
                         vga_increment();
                         break;
                         
                    case 'd':
                    case 'i':
                         _int = va_arg(arguments, int);
                         itoa(_int,10,_str);
                         printf(_str);
                         break;
                         
                    case 'f':
                         _float = (float)va_arg(arguments, double);
                         //need to implement 
                         printf("ftoa()");
                         break;
                         
                    case 's':
                         _str = va_arg(arguments, char*);
                         printf(_str);
                         break;
                         
                    default: 
                         vga_print_char(str[i]);
                         vga_increment();
                         break;
               }
               // Reset the formatting flag now that we're done.
               format_mode = 0;
          } else {
               // We are not in format mode, so we print the character
               vga_print_char(str[i]);
               vga_increment();
          }
     }
     
     // Close out the va_list
     va_end(arguments);
     return 0;
}