#include <ctype.h>
#include "scm.h"

static void read_error();
static int skip_whitespace_comments(scm_object *);
static scm_object* read_number(scm_object *, char);
static scm_object* read_symbol(scm_object *);
static scm_object* read_list(scm_object *);

void scm_read(scm_object *port)
{
    scm_object *obj = NULL;

    skip_whitespace_comments(port);
    
    char c = scm_getc(port);
    
    if(c == '#') {
        c = scm_getc(port);
        switch(c) {
        case 't':
            obj = scm_true;
            break;
        case 'f':
            obj = scm_false;
            break;
        default:
            obj = read_number(port, c);
        }
    }
    else if('0' <= c && c <= '9')
        obj = read_number(port, 10);
    else if(c == '(');
        obj = read_list(port);
    return obj;
}

static void read_error()
{
    printf("read error");
}

static scm_object* read_list(scm_object *port)
{
    /*
    scm_object *ret;
    scm_object *list;
    scm_object *o;
    char c;
    while((c = scm_getc(port)) != ')') {
        o = scm_read(port);
        list = scm_make_pair(o, scm_null);
        SCM_CAR(ret) = list;
        ret = list;
    }
    //if(c == '.')
    */
}

static scm_object* read_symbol(scm_object *port)
{
    
}

static scm_object* read_number(scm_object *port, char radixc)
{
    switch(radixc) {
    case 10:
        int n;
        char c;
        for(n = 0; isdigit(c = scm_getc(port) ); )
            n = 10 * n + (c - '0');
        return scm_make_integer(n);
    case 'b':
        // radix 2
    case 'o':
        // radix 8
    case 'd':
        // radix 10
    case 'x':
        // radix 16
    default:
        read_error();
        return;
    }
}

static int skip_whitespace_comments(scm_object *port)
{
    char c;
    do {
        c = scm_getc(port);
    } while(isspace(c));
}