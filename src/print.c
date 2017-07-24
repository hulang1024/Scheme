#include <stdio.h>
#include "scm.h"

void scm_write_pair(FILE *, scm_object *);
void scm_write(FILE *, scm_object *);
void scm_print(scm_object *);

void scm_write_pair(FILE *stream, scm_object *pair)
{
    fprintf(stream, "(");
    scm_object *o = pair;
    for(; SCM_PAIRP(o); o = SCM_CDR(o)) {
        scm_write(stream, SCM_CAR(o));
        fprintf(stream, " ");;
    }
    fprintf(stream, ". ");
    scm_write(stream, o);
    fprintf(stream, ")");
}

void scm_write(FILE *stream, scm_object *obj)
{
    switch(SCM_TYPE(obj)) {
        case scm_integer_type:
            fprintf(stream, "%d", SCM_INT_VAL(obj));
            break;
        case scm_float_type:
            fprintf(stream, "%f", SCM_FLOAT_VAL(obj));
            break;
        case scm_char_type:
            fprintf(stream, "%c", SCM_CHAR_VAL(obj));
            break;
        case scm_string_type:
            fprintf(stream, "%s", SCM_CHAR_STR_VAL(obj));
            break;
        case scm_symbol_type:
            fprintf(stream, "%s", SCM_SYMBOL_STR_VAL(obj));
            break;
        case scm_pair_type:
            scm_write_pair(stream, obj);
            break;
        case scm_primitive_type:
            fprintf(stream, "#[procedure:%s]", ((scm_primitive_proc *)obj)->name);
            break;
        case scm_compound_type:
            fprintf(stream, "#[procedure:%s]", ((scm_compound_proc *)obj)->name);
            break;
        case scm_namespace_type:
            fprintf(stream, "#[namespace]");
            break;
        case scm_void_type:
            break;
        default:
            if(SCM_NULLP(obj))
                fprintf(stream, "()");
            else if(SCM_BOOLP(obj))
                fprintf(stream, "#%c", SCM_TRUEP(obj) ? 't' : 'f');
    }
}

void scm_print(scm_object *obj)
{
    scm_write(stdout, obj);
}

void scm_println(scm_object *obj)
{
    scm_print(obj);
    fputc('\n', stdout);
}