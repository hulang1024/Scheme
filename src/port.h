#ifndef SCHEME_PORT_H
#define SCHEME_PORT_H

#include <stdio.h>
#include "scm.h"

typedef struct {
    scm_object o;
    FILE *f; // TODO: 不只是文件
} scm_input_port;

typedef struct {
    scm_object o;
    FILE *f;// TODO:
} scm_output_port;

#define scm_eofp(ch) ((ch) == EOF)

scm_object *scm_stdin_port;
scm_object *scm_stdout_port;

void scm_init_port(scm_env *);
scm_object* scm_make_stdout_port();
scm_object* scm_make_stdin_port();
scm_object* scm_make_file_output_port(FILE *);
scm_object* scm_make_file_input_port(FILE *);
scm_object* scm_close_output_port(scm_object *);
scm_object* scm_close_input_port(scm_object *);
int scm_getc(scm_object* port);
int scm_ungetc(int ch, scm_object* port);

#endif //SCHEME_PORT_H
