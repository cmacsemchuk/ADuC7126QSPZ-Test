/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <rt_misc.h>
#include <stdio.h>
#include <ADuC7128.H>                      /* ADuC7128 definitions     */

#define CR     0x0D


#pragma import(__use_no_semihosting_swi)

void _sys_exit(int return_code) 
{
label:  goto label;  /* endless loop */
}
/*
 * writes the character specified by c (converted to an unsigned char) to
 * the output stream pointed to by stream, at the position indicated by the
 * asociated file position indicator (if defined), and advances the
 * indicator appropriately. If the file position indicator is not defined,
 * the character is appended to the output stream.
 * Returns: the character written. If a write error occurs, the error
 *          indicator is set and fputc returns EOF.
 */
int fputc(int ch, FILE * stream )
{
   if(ch == '\n')
      {
      while(!(0x020==(COM0STA0 & 0x020)));
      COM0TX = CR;                     /* output CR */
      }

   while(!(0x020==(COM0STA0 & 0x020)));

   return(COM0TX = ch);
}
/*
 * obtains the next character (if present) as an unsigned char converted to
 * an int, from the input stream pointed to by stream, and advances the
 * associated file position indicator (if defined).
 * Returns: the next character from the input stream pointed to by stream.
 *          If the stream is at end-of-file, the end-of-file indicator is
 *          set and fgetc returns EOF. If a read error occurs, the error
 *          indicator is set and fgetc returns EOF.
 */
int fgetc(FILE * stream)
{
   while(!(0x01==(COM0STA0 & 0x01)))
   {}
   return (COM0RX);
}

