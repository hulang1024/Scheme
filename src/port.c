#include <stdio.h>
#include "scm.h"

typedef struct {
    scm_object o;
    FILE *f; // TODO: 不只是文件
} scm_input_port;

typedef struct {
    scm_object o;
    FILE *f;// TODO: 
} scm_ouput_port;


scm_object* scm_make_stdout_port()
{
    scm_object *o = scm_malloc_object(sizeof(scm_input_port));
    o->type = scm_input_port_type;
    o->f = stdout;
    return o;
}

scm_object* scm_make_stdin_port()
{
    scm_object *o = scm_malloc_object(sizeof(scm_ouput_port));
    o->type = scm_output_port_type;
    o->f = stdin;
    return o;
}

int scheme_getc(scm_object* port)
{
    return getc( ((scm_input_port *)port)->f );
}

int scheme_ungetc(scm_object* port)
{
    return ungetc( ((scm_input_port *)port)->f );
}
