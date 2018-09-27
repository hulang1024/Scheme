/**************************************************
* Author:  HuLang                                 *
* Notes:   Some functions about Console Control.  *
* License: Copyleft. Enjoy it Just for fun.       *
* Date:    2008-12-17 00:28:39                    *
***************************************************/

#ifndef _WIN32
#include "cc.h"

void delayMS(uint32 d)
{
   return ;
}

void clearText(void)
{
   return ;
}

int setTextColor(uint8 fColor)
{
   return 0;
}

PCCOLOR getTextColor(void)
{
   return 0;
}

int setBackColor(uint8 bColor)
{
   return 0;
}

PCCOLOR getBackColor(void)
{
   return 0;
}

int setColors(uint8 fColor, uint8 bColor)
{
   return 0;
}

int setSwapColors(int b)
{
   return 0;
}

int setUnderLine(int b)
{
   return 0;
}

uint8 getLineWidth(void)
{
   return 0;
}

uint8 getLineNum(void)
{
   return 0;
}

uint8 getCursorX(void)
{
   return 0;
}

uint8 getCursorY(void)
{
   return 0;
}

int gotoTextPos(uint8 x, uint8 y)
{
   return 0;
}

int setCursorVisible(int b)
{
   return 0;
}

int setCursorSize(uint8 s)
{
   return 0;
}

int getConsoleTitle(char *title, uint8 len)
{
   return 0;
}

int setConsoleTitle(char *title)
{
   return 0;
}

int fixConsoleSize(uint16 width, uint16 height)
{
   return 0;
}

int showConsoleInfo()
{
   return 0;
}

int simpleRing(uint16 freq, uint16 len)
{
   return 0;
}

uint16 jkGetKey(void)
{
   return 0;
}

int jkHasKey(void)
{
   return 0;
}

#endif
//End of pcc32.c
