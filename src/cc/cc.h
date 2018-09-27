/**************************************************
* Author:  HuLang                                 *
* Notes:   Some functions about Console Control.  *
* License: Copyleft. Enjoy it Just for fun.       *
* Date:    2008-12-17 00:28:39                    *
***************************************************/

#ifndef CC_H_INCLUDED
#define CC_H_INCLUDED

#ifdef _WIN32
  #include <stdio.h>
  #include <stdlib.h>
  #include <conio.h>
  #include <windows.h>
#else
  #include <termios.h>
  #include <unistd.h>
  #include <signal.h>
#endif

#ifndef COMMEN_TYPE
#define COMMEN_TYPE

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

#endif // COMMEN_TYPE

/* 按键定义 */
#define JK_FUNC_KEY  0x00
#define JK_CTRL_KEY  0xE0

#define JK_ESC       0x001B
#define JK_ENTER     0x000D
#define JK_SPACE     0x0020
#define JK_BKSPACE   0x0008
#define JK_TAB       0x0009

#define JK_CTRL_Z    0x001A
#define JK_CTRL_X    0x0018
#define JK_CTRL_C    0x0003
#define JK_CTRL_A    0x0001
#define JK_CTRL_S    0x0013
#define JK_CTRL_D    0x0004

#define JK_LEFT      0xE04B
#define JK_RIGHT     0xE04D
#define JK_UP        0xE048
#define JK_DOWN      0xE050
#define JK_INSERT    0xE052
#define JK_HOME      0xE047
#define JK_PGUP      0xE049
#define JK_DELETE    0xE053
#define JK_END       0xE04F
#define JK_PGDW      0xE051

#define JK_F1        0xFF3B
#define JK_F2        0xFF3C
#define JK_F3        0xFF3D
#define JK_F4        0xFF3E
#define JK_F5        0xFF3F
#define JK_F6        0xFF40
#define JK_F7        0xFF41
#define JK_F8        0xFF42
#define JK_F9        0xFF43
#define JK_F10       0xFF44
#define JK_F11       0xE085
#define JK_F12       0xE086

/* 控制台尺寸定义 */
#define MIN_CONSOLE_WIDTH   14
#define MIN_CONSOLE_HEIGHT  1

/* 颜色定义 */
typedef enum _PCCOLOR
{
   BLACK           =  0,   // 黑色
   BLUE            =  1,   // 蓝色
   GREEN           =  2,   // 绿色
   CYAN            =  3,   // 青色
   RED             =  4,   // 红色
   MAGENTA         =  5,   // 紫色
   BROWN           =  6,   // 褐色
   LIGHT_GRAY      =  7,   // 浅灰
   DARK_GRAY       =  8,   // 深灰
   LIGHT_BLUE      =  9,   // 亮蓝
   LIGHT_GREEN     = 10,   // 亮绿
   LIGHT_CYAN      = 11,   // 浅蓝
   LIGHT_RED       = 12,   // 亮红
   LIGHT_MAGENTA   = 13,   // 亮紫
   YELLOW          = 14,   // 黄色
   WHITE           = 15    // 白色
}PCCOLOR;

#ifdef __cplusplus
extern "C"
{
#endif

   /* 延时,以毫秒计 */
   void delayMS(uint32 d);
   /* 清除文字 */
   void clearText(void);
   /* 暂停,等待用户按键 */
   void pauseHere(void);
   /* 发出简单的响铃声(阻塞,慎用) */
   int simpleRing(uint16 freq, uint16 len);

   /* 设置文本颜色,0~15 */
   int setTextColor(uint8 fColor);
   /* 获取文本颜色,0~15 */
   PCCOLOR getTextColor(void);
   /* 设置文本背景颜色,0~15 */
   int setBackColor(uint8 bColor);
   /* 获取文本背景颜色,0~15 */
   PCCOLOR getBackColor(void);
   /* 设置文本及其背景颜色,0~15 */
   int setColors(uint8 fColor, uint8 bColor);
   /* 设置/取消前/背景颜色的交换解析 */
   int setSwapColors(int b);
   /* 设定/取消文字的下划线 */
   int setUnderLine(int b);

   /* 获取控制台文本行的最大长度[默认为80] */
   uint8 getLineWidth(void);
   /* 获取控制台文本行的最大行数 */
   uint8 getLineNum(void);


   /* 获取光标的横坐标[列数] */
   uint8 getCursorX(void);
   /* 获取光标的纵坐标[行数] */
   uint8 getCursorY(void);
   /* 屏幕光标定位,x为列(横),y为行(纵) */
   int gotoTextPos(uint8 x, uint8 y);
   /* 设置光标的可见性 */
   int setCursorVisible(int b);
   /* 设置光标的(厚度)尺寸,1-100 */
   int setCursorSize(uint8 s);

   /* 获取控制台的标题字符串 */
   int getConsoleTitle(char *title, uint8 len);
   /* 设置控制台的标题字符串 */
   int setConsoleTitle(char *title);

   /* 设置一个没有滚动条的控制台窗口尺寸 */
   int fixConsoleSize(uint16 width, uint16 height);
   /* 输出控制台的相关信息(仅作调试用) */
   int showConsoleInfo();

   /* 获取控制台的键输入 */
   uint16 jkGetKey(void);
   /* 判断控制台是否有键按下 */
   int jkHasKey(void);

#ifdef __cplusplus
}
#endif

#endif // CC_H_INCLUDED
