#include <ctype.h>
#include <string.h>
#include "read.h"
#include "bool.h"
#include "number.h"
#include "char.h"
#include "str.h"
#include "symbol.h"
#include "list.h"
#include "env.h"
#include "scm.h"

scm_object* scm_read(scm_object *);
static scm_object* read_char(scm_object *);
static scm_object* read_string(scm_object *);
static scm_object* read_number(scm_object *, char, int);
static scm_object* read_symbol(scm_object *, char);
static scm_object* read_quote(scm_object *);
static scm_object* read_list(scm_object *);
static void skip_whitespace_comments(scm_object *);
static void read_error();

static scm_object* read_prim(int, scm_object *[]);

int isodigit(char c) { return '0' <= c && c <= '7'; }

int isdelimiter(char c)
{
    if (isspace(c))
        return 1;
    switch (c) {
        case '(': case ')':
        case '[': case ']':
        case '{': case '}':
        case '"': case ';':
            return 1;
    }
    return 0;
}

int is_special_inital(char c)
{
    switch (c) {
        case '!': case '$': case '%':
        case '&': case '*': case '/':
        case ':': case '<': case '=':
        case '>': case '?': case '^':
        case '_': case '~':
            return 1;
    }
    return 0;
}

int is_peculiar_identifier(char c)
{
    switch (c) {
        case '+': case '-':
            // and case '...'
            return 1;
    }
    return 0;
}

int is_sepcial_subsequent(char c)
{
    switch (c) {
        case '+': case '-':
        case '.': case '@':
            return 1;
    }
    return 0;
}

void scm_init_read(scm_env *env)
{
    scm_add_prim(env, "read", read_prim, 0, 1);
}

static scm_object* read_prim(int argc, scm_object *argv[])
{
    return scm_read(scm_stdin_port);
}

scm_object* scm_read(scm_object *port)
{
    scm_object *obj = NULL;

    skip_whitespace_comments(port);

    char c = scm_getc(port);

    switch (c) {
        case '#':
            c = scm_getc(port);
            switch (c) {
                case 't':
                case 'T':
                    obj = scm_true;
                    break;
                case 'f':
                case 'F':
                    obj = scm_false;
                    break;
                case '\\':
                    obj = read_char(port);
                    break;
                default:
                    obj = read_number(port, c, 1);
            }
            break;
        case '-':
        case '+': {//TODO: +1a also a identifler
            char c1 = scm_getc(port);
            if (isdigit(c1)) {
                scm_ungetc(c1, port);
                obj = read_number(port, 10, c == '-' ? -1 : 1);
            } else {
                scm_ungetc(c1, port);
                obj = read_symbol(port, c);
            }
            break;
        }
        case '.': //TODO: .5 == 0.5
            obj = read_symbol(port, c);
            break;
        case '(':
        case '[':
        case '{':
            obj = read_list(port);
            break;
        case '"':
            obj = read_string(port);
            break;
        case '\'':
            obj = read_quote(port);
            break;
        default:
            if (scm_eofp(c)) {
                break;
            } else if (isdigit(c)) {
                scm_ungetc(c, port);
                obj = read_number(port, 10, 1);
            } else if (isalpha(c) || is_special_inital(c)) {
                obj = read_symbol(port, c);
            }
    }

    return obj;
}

static scm_object* read_quote(scm_object *port)
{
    return cons((scm_object *)scm_quote_symbol, cons(scm_read(port), scm_null));
}

static scm_object* read_list(scm_object *port)
{
    scm_object *head = scm_null, *prev = NULL, *curr;
    int found_dot = 0;
    scm_object *o;
    char c;

    while (1) {
        c = scm_getc(port);
        if(c == ')' || c == ']' || c == '}' || scm_eofp(c))
            break;

        scm_ungetc(c, port);
        o = scm_read(port);
        skip_whitespace_comments(port);

        if (prev != NULL) {
            if (NOT_SAME_OBJ(o, (scm_object *)scm_dot_symbol)) {
                if (!found_dot) {
                    curr = cons(o, scm_null);
                    SCM_CDR(prev) = curr;
                    prev = curr;
                } else {
                    SCM_CDR(prev) = o;
                }
            } else {
                found_dot = 1;
            }
        } else {
            head = prev = cons(o, scm_null);
        }
    }

    if (!SCM_NULLP(head)) {
        if (!found_dot || SCM_NULLP(curr)) {
            ((scm_pair *) head)->is_list_mark = 1;
        }
    }
    return head;
}

static scm_object* read_symbol(scm_object *port, char initch)
{
    #define SYMBOL_BUF_SIZE_INIT 10
    int buf_size = SYMBOL_BUF_SIZE_INIT;
    int buf_idx = 0;
    char *buf = (char*)malloc(SYMBOL_BUF_SIZE_INIT + 1);
    char c;

    buf[buf_idx++] = initch;
    while (1) {
        c = scm_getc(port);
        if (isdelimiter(c)) {
            scm_ungetc(c, port);
            break;
        } else if (scm_eofp(c)) {
            break;
        }
        if(buf_idx > buf_size) {
            buf_size += 10; // grow 10bytes
            buf = realloc(buf, buf_size);
        }
        buf[buf_idx++] = c;
    }
    buf[buf_idx] = '\0';

    return (scm_object *)scm_get_intern_symbol((const char *)buf);
}

static scm_object* read_number(scm_object *port, char radixc, int sign)
{
    #define NUMBER_BUF_SIZE_INIT 5
    #define APPEND_CH(c) if(buf_idx > buf_size) \
        { buf_size += 3; buf = realloc(buf, buf_size); } \
        buf[buf_idx++] = c;

    int buf_size = NUMBER_BUF_SIZE_INIT;
    int buf_idx = 0;
    char *buf = (char*)malloc(NUMBER_BUF_SIZE_INIT + 1);
    int dot = 0;
    char c;

    switch (radixc) {
        case 'b':
        case 'B':
            // radix 2
        case 'o':
        case 'O':
            // radix 8
        case 'd':
        case 'D':
        case 10:
            while (1) {
                c = scm_getc(port);
                if (isdigit(c)) {
                    APPEND_CH(c);
                } else if (c == '.') {
                    if(dot) {
                        read_error();
                        return NULL;
                    }
                    dot = 1;
                    APPEND_CH(c);
                } else if (scm_eofp(c)) {
                    break;
                } else {
                    scm_ungetc(c, port);
                    break;
                }
            }
            buf[buf_idx] = '\0';

            if (dot) {
                double num = atof(buf);
                if (sign < 0) num *= sign;
                return scm_make_float(num);
            } else {
                int num = atol(buf);
                if (sign < 0) num *= sign;
                return scm_make_integer(num);
            }
            break;
        case 'x':
        case 'X':
            // radix 16
        default:
            read_error();
            return NULL;
    }
}

static scm_object* read_char(scm_object *port)
{
    #define CHARS_BUF_SIZE_INIT 1

    int buf_size = CHARS_BUF_SIZE_INIT;
    int buf_idx = 0;
    char *buf = (char*)malloc(CHARS_BUF_SIZE_INIT + 1);
    char c;

    while (1) {
        c = scm_getc(port);
        if (isdelimiter(c)) {
            scm_ungetc(c, port);
            break;
        } else if (scm_eofp(c)) {
            break;
        }
        if (buf_idx > buf_size) {
            buf_size += 2;
            buf = realloc(buf, buf_size);
        }
        buf[buf_idx++] = c;
    }
    if (buf_idx == 1) {
        c = *buf;
    } else {
        buf[buf_idx] = '\0';
        if (stricmp(buf, "space") == 0)
            c = ' ';
        else if (stricmp(buf, "newline") == 0)
            c = '\n';
    }
    return scm_make_char(c);
}

static scm_object* read_string(scm_object *port)
{
    #define STR_BUF_SIZE_INIT 50

    int buf_size = STR_BUF_SIZE_INIT;
    int buf_idx = 0;
    char *buf = (char*)malloc(STR_BUF_SIZE_INIT + 1);
    char ch;

    while (1) {
        ch = scm_getc(port);
        // escape sequence handling
        if (ch == '\\') {
            ch = scm_getc(port);
            switch (ch) {
                case '\\': case '\"': case '\'': break;
                case 'a': ch = '\a'; break;
                case 'b': ch = '\b'; break;
                case 'e': ch = '\33'; break; /* escape */
                case 'f': ch = '\f'; break;
                case 'n': ch = '\n'; break;
                case 'r': ch = '\r'; break;
                case 't': ch = '\t'; break;
                case 'v': ch = '\v'; break;
                case 'x':
                    // TODO:
                case 'u':
                case 'U':
                    // TODO:
                default:
                    //if(isodigit(ch))
                    ;
            }
        } else if (ch == '"') {
            break;
        } else if (scm_eofp(ch)) {
            break;
        }
        if (buf_idx > buf_size) {
            buf_size += 20;
            buf = realloc(buf, buf_size);
        }
        buf[buf_idx++] = ch;
    }
    buf[buf_idx] = '\0';

    return scm_make_string(buf);
}

static void skip_whitespace_comments(scm_object *port)
{
    char c;
    while (1) {
        c = scm_getc(port);
        if (isspace(c))
            continue;
        else if (c == ';') {
            while(1) {
                c = scm_getc(port);
                if (c == '\n' || c == '\r')
                    break;
                else if (scm_eofp(c))
                    return;
            }
        } else
            break;
    }
    scm_ungetc(c, port);
}

static void read_error()
{
    printf("read error");
}
