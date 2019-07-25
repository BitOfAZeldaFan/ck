// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     std/string.c
// = Purpose:  C Standard Library - Collection of essential C tools
// ========================================================================== //

#include <string.h>
#include <stdlib.h>
#include <types.h>

volatile int itoa_case = UPPERCASE;
volatile byte itoa_long = true;
static const char place_value[2][16] = { 
      "0123456789ABCDEF",
      "0123456789abcdef",
};

static const char bytes_sizes[6] = "BKMGTP";

size_t strlen(const char *str)
{
     size_t size=0;
     while(str[size] != '\0') {
          ++size;
          if(size > 255) return size;
     }
     return size;
}

void reverse(char *s)
{
     size_t length = strlen(s);
     char new_str[length];
     
     for(size_t i=0; i<length; i++) {
          new_str[length-i-1] = s[i];
     }
     for(size_t i=0; i<length; i++) {
          s[i] = new_str[i];
     }
     s[length] = '\0';
}


/**  @Function:      atoi()
 *   @Arguments:     character string
 *   @Returns:       integer
 *   
 *  Converts a C string into an integer using powers of 10.
 */
 
/**  @Function:     itoa()
 *   @Arguments:    Integer to convert, number base, and pointer to string
 *   @Returns:      pointer to a character string
 *
 *   Pretty much just converts an integer into the string pointer provided.
 */
void itoa(int number, base_t base, char str[128])
{  
     if(base == BYTES) {
          //itoa_bytes(number, str);
          return;
     }
     else if(base == BOOLEAN) {
          str = number?"true":"false";
          return;
     }
     else if(base == 0 || number == 0) {
          str[0] = '0';
          str[1] = '\0';
          return;
     }
     else {
          int pos=0, i;

          while(number > 0)
          {
               i = number % base;
               str[pos++] = place_value[itoa_case][i];
               number = number / base;
          }
          if(base == HEX && itoa_long) {
               str[pos++] = 'x';
               str[pos++] = '0';
          }
          str[pos] = 0;
          if(pos) reverse(str);
     }
}