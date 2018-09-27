/********************************************************
* Author:  RedOC                                        *
* Email:   RedOC@foxmail.com                            *
* Notes:   Some functions about Win32 Console Control.  *
* License: Copyleft. Enjoy it Just for fun.             *
* Date:    2008-12-17 00:28:39                          *
********************************************************/
#ifdef _WIN32
#include "cc.h"

void delayMS(uint32 d)
{
   Sleep(d);
   return ;
}

void clearText(void)
{
   system("cls");
   return ;
}

int setTextColor(uint8 fColor)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(hd, &csbInfo);
   return SetConsoleTextAttribute(hd, fColor | (csbInfo.wAttributes&~0x0F));
}

PCCOLOR getTextColor(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
   return (PCCOLOR)(csbInfo.wAttributes&0x0F);
}

int setBackColor(uint8 bColor)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(hd, &csbInfo);
   return SetConsoleTextAttribute(hd, (bColor << 4) | (csbInfo.wAttributes&~0xF0));
}

PCCOLOR getBackColor(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
   return (PCCOLOR)((csbInfo.wAttributes&0xF0) >> 4);
}

int setColors(uint8 fColor, uint8 bColor)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(hd, &csbInfo);
   return SetConsoleTextAttribute(hd, fColor | (bColor << 4) | (csbInfo.wAttributes&~0xFF));
}

int setSwapColors(int b)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(hd, &csbInfo);
   if (!!b)
      return SetConsoleTextAttribute(hd, csbInfo.wAttributes | 0x4000);
   else
      return SetConsoleTextAttribute(hd, csbInfo.wAttributes & ~0x4000);
}

int setUnderLine(int b)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(hd, &csbInfo);
   if (!!b)
      return SetConsoleTextAttribute(hd, csbInfo.wAttributes | 0x8000);
   else
      return SetConsoleTextAttribute(hd, csbInfo.wAttributes & ~0x8000);
}

uint8 getLineWidth(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
   return csbInfo.dwSize.X;
}

uint8 getLineNum(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
   return csbInfo.dwSize.Y;
}

uint8 getCursorX(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
   return csbInfo.dwCursorPosition.X;
}

uint8 getCursorY(void)
{
   CONSOLE_SCREEN_BUFFER_INFO csbInfo;
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
   return csbInfo.dwCursorPosition.Y;
}

int gotoTextPos(uint8 x, uint8 y)
{
   COORD cd;
   cd.X = x;
   cd.Y = y;
   return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

int setCursorVisible(int b)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO ccInfo;
   GetConsoleCursorInfo(hd, &ccInfo);
   ccInfo.bVisible = !!b;
   return SetConsoleCursorInfo(hd, &ccInfo);
}

int setCursorSize(uint8 s)
{
   HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO ccInfo;
   GetConsoleCursorInfo(hd, &ccInfo);
   ccInfo.dwSize = s;
   return SetConsoleCursorInfo(hd, &ccInfo);
}

int getConsoleTitle(char *title, uint8 len)
{
   return GetConsoleTitle(title, len);
}

int setConsoleTitle(char *title)
{
   return SetConsoleTitle(title);
}

int fixConsoleSize(uint16 width, uint16 height)
{
   int ret = 0;
   int fixX = 0, fixY = 0;
   COORD cd;
   SMALL_RECT srctWindow;
   CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
   HANDLE hWin = GetStdHandle(STD_OUTPUT_HANDLE);

   if (!hWin)
      return 0;

   // 调整最小范围
   fixX = (width < MIN_CONSOLE_WIDTH) ? MIN_CONSOLE_WIDTH : width;
   fixY = (height < MIN_CONSOLE_HEIGHT) ? MIN_CONSOLE_HEIGHT : height;

   // 先将BUF尺寸扩大到最大
   cd.X = 512;
   cd.Y = 512;
   ret = SetConsoleScreenBufferSize(hWin, cd);
   if (!ret)
      return ret;
   //puts("Set Max Buf Error.");

   // 测试屏幕允许的最大尺寸
   GetConsoleScreenBufferInfo(hWin, &csbiInfo);
   cd = csbiInfo.dwMaximumWindowSize;
   //printf("Max Win Size[%d, %d].\n", cd.X, cd.Y);
   fixX = (fixX > cd.X) ? cd.X : fixX;
   fixY = (fixY > cd.Y) ? cd.Y : fixY;
   //printf("Fix Win Size[%d, %d].\n", fixX, fixY);

   // 确定适当的窗口尺寸
   srctWindow.Left = 0;
   srctWindow.Right = fixX - 1;
   srctWindow.Top = 0;
   srctWindow.Bottom = fixY - 1;
   ret = SetConsoleWindowInfo(hWin, 1, &srctWindow);
   if (!ret)
      return ret;
   //puts("Set Size Error.");

   // 确定适当的BUF尺寸
   cd.X = fixX;
   cd.Y = fixY;
   ret = SetConsoleScreenBufferSize(hWin, cd);
   if (!ret)
      return ret;

   //printf("Fix Win Size[%d, %d]: %d.\n", fixX, fixY, ret);
   Sleep(100);
   return ret;
}

int showConsoleInfo()
{
   int ret;
   HANDLE hWin = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

   ret = GetConsoleScreenBufferInfo(hWin, &csbiInfo);
   if (ret)
   {
      printf("Buffer Size: [X]%d - [Y]%d\n", csbiInfo.dwSize.X, csbiInfo.dwSize.Y);
      printf("Cursor Pos : [X]%d - [Y]%d\n", csbiInfo.dwCursorPosition.X, csbiInfo.dwCursorPosition.Y);
      printf("Attributes : %d\n", csbiInfo.wAttributes);
      printf("Current Win: [L]%d - [R]%d - [T]%d - [B]%d\n", \
             csbiInfo.srWindow.Left, csbiInfo.srWindow.Right, csbiInfo.srWindow.Top, csbiInfo.srWindow.Bottom);
      printf("Maximum Win: [X]%d - [Y]%d\n", csbiInfo.dwMaximumWindowSize.X, csbiInfo.dwMaximumWindowSize.Y);
      puts("Over.");
   }
   return ret;
}

int simpleRing(uint16 freq, uint16 len)
{
   return Beep(freq, len);
}

uint16 jkGetKey(void)
{
   uint16 rk = 0;
   uint8 k = getch();
   if (k == JK_FUNC_KEY)
   {
      k = getch();
      rk = 0xFF00 | k;
      return rk;
   }
   if (k == JK_CTRL_KEY)
   {
      k = getch();
      rk = 0xE000 | k;
      return rk;
   }
   rk = 0x0000 | k;
   return rk;
}

int jkHasKey(void)
{
   return (kbhit());
}

#endif
//End of pcc32.c
