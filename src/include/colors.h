// ========================================================================== //
// = C/K Project 2019 - Bryan Webb
// = File:     include/colors.h
// = Purpose:  Contains various definitions for VGA colors
// ========================================================================== //

#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED

#include <types.h>

#define GRAY GREY

enum __COLORS
{
     /* Basic single colors */
     BLACK, BLUE, GREEN, CYAN, RED, PURPLE, BROWN, GREY, DGREY,
     LBLUE, LGREEN, LCYAN, LRED, LPURPLE, YELLOW, WHITE,
     
     /* Defined palettes */
     C_MSDOS        = 0x07,  	// Grey on Black
     C_BLUESCR      = 0x1f,   // White on Blue
	C_CRITICAL     = 0x5f,	// White on Red
	C_ERROR        = 0x0f,   // Red on Grey
};

typedef byte color_t;

#endif
