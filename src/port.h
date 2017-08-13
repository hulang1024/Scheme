#ifndef SCHEME_PORT_H
#define SCHEME_PORT_H

#include <stdio.h>
#include "scm.h"

#define scm_eofp(ch) ((ch) == EOF)

#define INPUT_PORT_GETC_FUNC(o) (((scm_input_port *)o)->getc)
#define INPUT_PORT_UNGETC_FUNC(o) (((scm_input_port *)o)->ungetc)
#define OUTPUT_PORT_PUTC_FUNC(o) (((scm_output_port *)o)->putc)
#define OUTPUT_PORT_WRITE_CSTR_FUNC(o) (((scm_output_port *)o)->write_cstr)
#define FILE_INPUT_PORT_FILE(o) (((scm_file_input_port *)o)->f)
#define FILE_OUTPUT_PORT_FILE(o) (((scm_file_output_port *)o)->f)
#define CHAR_STRING_INPUT_PORT_BUF(o) (((scm_char_string_input_port *)o)->buf)
#define CHAR_STRING_OUTPUT_PORT_BUF(o) (((scm_char_string_output_port *)o)->buf)

struct scm_input_port;
struct scm_output_port;
typedef struct scm_input_port scm_input_port;
typedef struct scm_output_port scm_output_port;

typedef int (*getc_fn)(scm_input_port *);
typedef int (*ungetc_fn)(scm_input_port *, int);
typedef int (*putc_fn)(scm_output_port *, int);
typedef int (*write_cstr_fn)(scm_output_port *, const char *);
typedef int (*close_fn)(scm_input_port *);

struct scm_input_port {
    scm_object o;
    getc_fn getc;
    ungetc_fn ungetc;
    close_fn close;
};

struct scm_output_port {
    scm_object o;
    putc_fn putc;
    write_cstr_fn write_cstr;
};

typedef struct {
    scm_input_port in;
    FILE *f;
} scm_file_input_port;

typedef struct {
    scm_output_port out;
    FILE *f;
} scm_file_output_port;

typedef struct {
    scm_input_port in;
    char* buf;
    int size;
    int cnt;
} scm_char_string_input_port;

typedef struct {
    scm_output_port out;
    char* buf;
    int size;
    int cnt;
} scm_char_string_output_port;

scm_object *scm_stdin_port;
scm_object *scm_stdout_port;

void scm_init_port(scm_env *);

int scm_getc(scm_object* port);
int scm_ungetc(int ch, scm_object* port);
int scm_putc(scm_object* port, int c);
int scm_write_cstr(scm_object* port, const char *s);
scm_object* scm_close_output_port(scm_object *);
scm_object* scm_close_input_port(scm_object *);

scm_object* scm_make_stdout_port();
scm_object* scm_make_stdin_port();

scm_object* scm_make_file_output_port(FILE *);
scm_object* scm_make_file_input_port(FILE *);

scm_object* scm_make_char_string_output_port(int size);
scm_object* scm_make_char_string_input_port(const char *, int size);


#endif //SCHEME_PORT_H
