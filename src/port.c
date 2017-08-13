#include <string.h>
#include "port.h"
#include "bool.h"

void scm_init_port(scm_env *env)
{
    scm_stdin_port = scm_make_stdin_port();
    scm_stdout_port = scm_make_stdout_port();
}

int scm_getc(scm_object* port)
{
    return INPUT_PORT_GETC_FUNC(port)((scm_input_port *)port);
}

int scm_ungetc(int ch, scm_object* port)
{
    return INPUT_PORT_UNGETC_FUNC(port)((scm_input_port *)port, ch);
}

int scm_putc(scm_object* port, int c)
{
    return OUTPUT_PORT_PUTC_FUNC(port)((scm_output_port *)port, c);
}

int scm_write_cstr(scm_object* port, const char *s)
{
    return OUTPUT_PORT_WRITE_CSTR_FUNC(port)((scm_output_port *)port, s);
}

scm_object* scm_close_output_port(scm_object *port)
{
    fclose(FILE_OUTPUT_PORT_FILE(port));
    return scm_void;
}

scm_object* scm_close_input_port(scm_object *port)
{
    fclose(FILE_INPUT_PORT_FILE(port));
    return scm_void;
}

/* std out/in  */

scm_object* scm_make_stdout_port()
{
    return scm_make_file_output_port(stdout);
}

scm_object* scm_make_stdin_port()
{
    return scm_make_file_input_port(stdin);
}

/* file */
static int file_getc(scm_input_port *);
static int file_ungetc(scm_input_port *, int);
static int file_putc(scm_output_port *, int);
static int file_write_cstr(scm_output_port *, const char *);
/* char-string */
/* file */
static int char_string_getc(scm_input_port *);
static int char_string_ungetc(scm_input_port *, int);
static int char_string_putc(scm_output_port *, int);
static int char_string_write_cstr(scm_output_port *, const char *);

scm_object* scm_make_file_output_port(FILE *f)
{
    scm_object *port = scm_malloc_object(sizeof(scm_file_output_port));
    port->type = scm_output_port_type;
    FILE_OUTPUT_PORT_FILE(port) = f;
    OUTPUT_PORT_PUTC_FUNC(port) = file_putc;
    OUTPUT_PORT_WRITE_CSTR_FUNC(port) = file_write_cstr;
    return port;
}

scm_object* scm_make_file_input_port(FILE *f)
{
    scm_object *port = scm_malloc_object(sizeof(scm_file_input_port));
    port->type = scm_input_port_type;
    FILE_INPUT_PORT_FILE(port) = f;
    INPUT_PORT_GETC_FUNC(port) = file_getc;
    INPUT_PORT_UNGETC_FUNC(port) = file_ungetc;
    return port;
}

scm_object* scm_make_char_string_output_port(int size)
{
    scm_object *port = scm_malloc_object(sizeof(scm_char_string_output_port));
    port->type = scm_output_port_type;

    size = size > -1 ? size + 1 : 10;
    ((scm_char_string_output_port *)port)->size = size;
    CHAR_STRING_OUTPUT_PORT_BUF(port) = malloc(size * sizeof(char));
    ((scm_char_string_output_port *)port)->cnt = 0;

    OUTPUT_PORT_PUTC_FUNC(port) = char_string_putc;
    OUTPUT_PORT_WRITE_CSTR_FUNC(port) = char_string_write_cstr;
    return port;
}

scm_object* scm_make_char_string_input_port(const char *s, int size)
{
    scm_object *port = scm_malloc_object(sizeof(scm_char_string_input_port));
    port->type = scm_input_port_type;

    size = size > -1 ? size + 1: 20;
    ((scm_char_string_input_port *)port)->size = size;
    CHAR_STRING_INPUT_PORT_BUF(port) = calloc(size, sizeof(char));
    strcpy(CHAR_STRING_INPUT_PORT_BUF(port), s);
    ((scm_char_string_input_port *)port)->cnt = 0;

    INPUT_PORT_GETC_FUNC(port) = char_string_getc;
    INPUT_PORT_UNGETC_FUNC(port) = char_string_ungetc;
    return port;
}

static int file_getc(scm_input_port *port)
{
    return fgetc(FILE_INPUT_PORT_FILE(port));
}

static int file_ungetc(scm_input_port *port, int c)
{
    return ungetc(c, FILE_INPUT_PORT_FILE(port));
}

static int file_putc(scm_output_port *port, int c)
{
    return fputc(c, FILE_OUTPUT_PORT_FILE(port));
}

static int file_write_cstr(scm_output_port *port, const char *s)
{
    return fprintf(FILE_OUTPUT_PORT_FILE(port), "%s", s);
}

static int char_string_getc(scm_input_port *port)
{
    int c = CHAR_STRING_INPUT_PORT_BUF(port)[((scm_char_string_input_port *)port)->cnt];
    if (c) {
        ((scm_char_string_input_port *)port)->cnt++;
        return c;
    } else
        return EOF;
}

static int char_string_ungetc(scm_input_port *port, int c)
{
    if (c == EOF)
        return EOF;
    ((scm_char_string_input_port *)port)->cnt--;
    CHAR_STRING_INPUT_PORT_BUF(port)[((scm_char_string_input_port *)port)->cnt] = c;
    return c;
}

static int char_string_putc(scm_output_port *port, int c)
{
    char *buf = CHAR_STRING_OUTPUT_PORT_BUF(port);
    int size = ((scm_char_string_output_port *)port)->size;
    int cnt = ((scm_char_string_output_port *)port)->cnt;

    if (cnt >= size) {
        size += 10;
        CHAR_STRING_OUTPUT_PORT_BUF(port) = realloc(buf, size);
        memset(buf + cnt, 0, sizeof(char));
    }

    return buf[cnt++] = c;
}

static int char_string_write_cstr(scm_output_port *port, const char *s)
{
    char *buf = CHAR_STRING_OUTPUT_PORT_BUF(port) + ((scm_char_string_output_port *)port)->cnt;

    while (*s) {
        *buf++ = *s++;
    }

    return 0;
}