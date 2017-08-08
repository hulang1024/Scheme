#include "libpcc32.h"
#include "../pcc32.h"
#include "../bool.h"
#include "../number.h"
#include "../symbol.h"
#include "../env.h"


#define S_CHAR_PTR_VAL SCM_CHAR_STR_VAL
#define S_INT_VAL SCM_INT_VAL
#define S_UINT32_VAL SCM_INT_VAL
#define S_UINT16_VAL SCM_INT_VAL
#define S_UINT8_VAL SCM_INT_VAL

#define RETURN_VOID() return scm_void;
#define RETURN_INT(v) return scm_make_integer(v);
#define RETURN_UINT16(v) return scm_make_integer(v);
#define RETURN_UINT8(v) return scm_make_integer(v);
#define RETURN_BOOL(v) return SCM_BOOL(v);

#define DEFINE_PRIM(name) \
   static scm_object* name##_prim(int argc, scm_object *argv[])

#define ADD_PRIM(name, mina, maxa) \
   scm_add_prim(env, "pcc-"#name, name##_prim, mina, maxa);
#define ADD_NULLARY_PRIM(name) ADD_PRIM(name, 0, 0)
#define ADD_UNARY_PRIM(name) ADD_PRIM(name, 1, 1)
#define ADD_BINARY_PRIM(name) ADD_PRIM(name, 2, 2)
#define ADD_TERNARY_PRIM(name) ADD_PRIM(name, 3, 3)
#define ADD_INT_CONSTANT(name) \
    scm_env_add_binding(env, scm_get_intern_symbol("pcc-"#name), scm_make_integer(name))


/* 函数定义 */
DEFINE_PRIM(delayMS)
{
    delayMS(S_UINT32_VAL(argv[0]));
    RETURN_VOID();
}

DEFINE_PRIM(clearText)
{
    clearText();
    RETURN_VOID();
}

DEFINE_PRIM(simpleRing)
{
    RETURN_INT(simpleRing(S_UINT16_VAL(argv[0]), S_UINT16_VAL(argv[1])));
}

DEFINE_PRIM(setTextColor)
{
    RETURN_INT(setTextColor(S_UINT8_VAL(argv[0])));
}

DEFINE_PRIM(getTextColor)
{
    RETURN_INT(getTextColor());
}

DEFINE_PRIM(setBackColor)
{
    RETURN_INT(setBackColor(S_UINT8_VAL(argv[0])));
}

DEFINE_PRIM(getBackColor)
{
    RETURN_INT(getBackColor());
}

DEFINE_PRIM(setColors)
{
    RETURN_INT(setColors(S_UINT8_VAL(argv[0]), S_UINT8_VAL(argv[1])));
}

DEFINE_PRIM(setSwapColors)
{
    RETURN_INT(setSwapColors(S_INT_VAL(argv[0])));
}

DEFINE_PRIM(setUnderLine)
{
    RETURN_INT(setUnderLine(S_INT_VAL(argv[0])));
}

DEFINE_PRIM(getLineWidth)
{
    RETURN_UINT8(getLineWidth());
}

DEFINE_PRIM(getLineNum)
{
    RETURN_UINT8(getLineNum());
}

DEFINE_PRIM(getCursorX)
{
    RETURN_UINT8(getCursorX());
}

DEFINE_PRIM(getCursorY)
{
    RETURN_UINT8(getCursorY());
}

DEFINE_PRIM(gotoTextPos)
{
    RETURN_INT(gotoTextPos(S_UINT8_VAL(argv[0]), S_UINT8_VAL(argv[1])));
}

DEFINE_PRIM(setCursorVisible)
{
    RETURN_INT(setCursorVisible(S_INT_VAL(argv[0])));
}

DEFINE_PRIM(setCursorSize)
{
    RETURN_INT(setCursorSize(S_UINT8_VAL(argv[0])));
}

DEFINE_PRIM(getConsoleTitle)
{
    RETURN_INT(getConsoleTitle(S_CHAR_PTR_VAL(argv[0]), S_UINT8_VAL(argv[1])));
}

DEFINE_PRIM(setConsoleTitle)
{
    RETURN_INT(setConsoleTitle(S_CHAR_PTR_VAL(argv[0])));
}

DEFINE_PRIM(fixConsoleSize)
{
    RETURN_INT(fixConsoleSize(S_UINT16_VAL(argv[0]), S_UINT16_VAL(argv[1])));
}

DEFINE_PRIM(showConsoleInfo)
{
    RETURN_INT(showConsoleInfo());
}

DEFINE_PRIM(jkGetKey)
{
    RETURN_UINT16(jkGetKey());
}
DEFINE_PRIM(jkHasKey)
{
    RETURN_BOOL(jkHasKey());
}

void scm_init_libpcc32(scm_env *env)
{
    /* 按键定义 */
    ADD_INT_CONSTANT(JK_FUNC_KEY);
    ADD_INT_CONSTANT(JK_CTRL_KEY);

    ADD_INT_CONSTANT(JK_ESC);
    ADD_INT_CONSTANT(JK_ENTER);
    ADD_INT_CONSTANT(JK_SPACE);
    ADD_INT_CONSTANT(JK_BKSPACE);
    ADD_INT_CONSTANT(JK_TAB);

    ADD_INT_CONSTANT(JK_CTRL_Z);
    ADD_INT_CONSTANT(JK_CTRL_X);
    ADD_INT_CONSTANT(JK_CTRL_C);
    ADD_INT_CONSTANT(JK_CTRL_A);
    ADD_INT_CONSTANT(JK_CTRL_S);
    ADD_INT_CONSTANT(JK_CTRL_D);

    ADD_INT_CONSTANT(JK_LEFT);
    ADD_INT_CONSTANT(JK_RIGHT);
    ADD_INT_CONSTANT(JK_UP);
    ADD_INT_CONSTANT(JK_DOWN);
    ADD_INT_CONSTANT(JK_INSERT);
    ADD_INT_CONSTANT(JK_HOME);
    ADD_INT_CONSTANT(JK_PGUP);
    ADD_INT_CONSTANT(JK_DELETE);
    ADD_INT_CONSTANT(JK_END);
    ADD_INT_CONSTANT(JK_PGDW);

    ADD_INT_CONSTANT(JK_F1);
    ADD_INT_CONSTANT(JK_F2);
    ADD_INT_CONSTANT(JK_F3);
    ADD_INT_CONSTANT(JK_F4);
    ADD_INT_CONSTANT(JK_F5);
    ADD_INT_CONSTANT(JK_F6);
    ADD_INT_CONSTANT(JK_F7);
    ADD_INT_CONSTANT(JK_F8);
    ADD_INT_CONSTANT(JK_F9);
    ADD_INT_CONSTANT(JK_F10);
    ADD_INT_CONSTANT(JK_F11);
    ADD_INT_CONSTANT(JK_F12);

    /* 控制台尺寸定义 */
    ADD_INT_CONSTANT(MIN_CONSOLE_WIDTH);
    ADD_INT_CONSTANT(MIN_CONSOLE_HEIGHT);

    /* 颜色定义 */
    ADD_INT_CONSTANT(BLACK);
    ADD_INT_CONSTANT(BLUE);
    ADD_INT_CONSTANT(GREEN);
    ADD_INT_CONSTANT(CYAN);
    ADD_INT_CONSTANT(RED);
    ADD_INT_CONSTANT(MAGENTA);
    ADD_INT_CONSTANT(BROWN);
    ADD_INT_CONSTANT(LIGHT_GRAY);
    ADD_INT_CONSTANT(DARK_GRAY);
    ADD_INT_CONSTANT(LIGHT_BLUE);
    ADD_INT_CONSTANT(LIGHT_GREEN);
    ADD_INT_CONSTANT(LIGHT_CYAN);
    ADD_INT_CONSTANT(LIGHT_RED);
    ADD_INT_CONSTANT(LIGHT_MAGENTA);
    ADD_INT_CONSTANT(YELLOW);
    ADD_INT_CONSTANT(WHITE);

    /* 过程定义 */
    ADD_UNARY_PRIM(delayMS);
    ADD_NULLARY_PRIM(clearText);
    ADD_BINARY_PRIM(simpleRing);
    ADD_UNARY_PRIM(setTextColor);
    ADD_NULLARY_PRIM(getTextColor);
    ADD_UNARY_PRIM(setBackColor);
    ADD_NULLARY_PRIM(getBackColor);
    ADD_BINARY_PRIM(setColors);
    ADD_UNARY_PRIM(setSwapColors);
    ADD_UNARY_PRIM(setUnderLine);
    ADD_NULLARY_PRIM(getLineWidth);
    ADD_NULLARY_PRIM(getLineNum);
    ADD_NULLARY_PRIM(getCursorX);
    ADD_NULLARY_PRIM(getCursorY);
    ADD_BINARY_PRIM(gotoTextPos);
    ADD_UNARY_PRIM(setCursorVisible);
    ADD_UNARY_PRIM(setCursorSize);
    ADD_BINARY_PRIM(getConsoleTitle);
    ADD_UNARY_PRIM(setConsoleTitle);
    ADD_BINARY_PRIM(fixConsoleSize);
    ADD_NULLARY_PRIM(showConsoleInfo);
    ADD_NULLARY_PRIM(jkGetKey);
    ADD_NULLARY_PRIM(jkHasKey);
}
