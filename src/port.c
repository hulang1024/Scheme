#include "port.h"

void scm_init_port(scm_env *env)
{
    scm_stdin_port = scm_make_stdin_port();
    scm_stdout_port = scm_make_stdout_port();
}

scm_object* scm_make_stdout_port()
{
    scm_object *port = scm_malloc_object(sizeof(scm_output_port));
    port->type = scm_output_port_type;
    ((scm_output_port *)port)->f = stdout;
    return port;
}

scm_object* scm_make_stdin_port()
{
    scm_object *port = scm_malloc_object(sizeof(scm_input_port));
    port->type = scm_input_port_type;
    ((scm_input_port *)port)->f = stdin;
    return port;
}

scm_object* scm_make_file_output_port(FILE *f)
{
    scm_object *port = scm_malloc_object(sizeof(scm_output_port));
    port->type = scm_output_port_type;
    ((scm_output_port *)port)->f = f;
    return port;
}

scm_object* scm_make_file_input_port(FILE *f)
{
    scm_object *port = scm_malloc_object(sizeof(scm_input_port));
    port->type = scm_input_port_type;
    ((scm_input_port *)port)->f = f;
    return port;
}

int scm_getc(scm_object* port)
{
    return getc( ((scm_input_port *)port)->f );
}

int scm_ungetc(int ch, scm_object* port)
{
    return ungetc(ch, ((scm_input_port *)port)->f );
}