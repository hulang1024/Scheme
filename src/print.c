#include <stdio.h>
#include "scm.h"

void scm_write_pair(scm_object *, scm_object *);
void scm_write(scm_object *, scm_object *);
void scm_print(scm_object *);

void scm_write_pair(scm_object *port, scm_object *pair)
{
    FILE* f = ((scm_output_port *)port)->f;// TODO:
    
    fprintf(f, "(");
    scm_object *o = pair;
    for(; SCM_PAIRP(o); o = SCM_CDR(o)) {
        scm_write(f, SCM_CAR(o));
        fprintf(f, " ");;
    }
    fprintf(f, ". ");
    scm_write(f, o);
    fprintf(f, ")");
}

void scm_write(scm_object *port, scm_object *obj)
{
    FILE* f = ((scm_output_port *)port)->f;// TODO:
    
    switch(SCM_TYPE(obj)) {
        case scm_integer_type:
            fprintf(f, "%d", SCM_INT_VAL(obj));
            break;
        case scm_float_type:
            fprintf(f, "%f", SCM_FLOAT_VAL(obj));
            break;
        case scm_char_type:
            fprintf(f, "%c", SCM_CHAR_VAL(obj));
            break;
        case scm_string_type:
            fprintf(f, "%s", SCM_CHAR_STR_VAL(obj));
            break;
        case scm_symbol_type:
            fprintf(f, "%s", SCM_SYMBOL_STR_VAL(obj));
            break;
        case scm_pair_type:
            scm_write_pair(f, obj);
            break;
        case scm_primitive_type:
            fprintf(f, "#[procedure:%s]", ((scm_primitive_proc *)obj)->name);
            break;
        case scm_compound_type:
            fprintf(f, "#[procedure:%s]", ((scm_compound_proc *)obj)->name);
            break;
        case scm_namespace_type:
            fprintf(f, "#[namespace]");
            break;
        case scm_void_type:
            break;
        default:
            if(SCM_NULLP(obj))
                fprintf(f, "()");
            else if(SCM_BOOLP(obj))
                fprintf(f, "#%c", SCM_TRUEP(obj) ? 't' : 'f');
    }
}

void scm_print(scm_object *port, scm_object *obj)
{
    scm_write(port, obj);
}

void scm_println(scm_object *obj)
{
    scm_print(obj);
    fputc('\n', stdout);
}