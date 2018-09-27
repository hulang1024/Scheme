#include <stdio.h>
#include <string.h>
#include "error.h"
#include "print.h"
#include "eval.h"
#include <setjmp.h>
#include "cc/cc.h"

void scm_print_error(const char *info)
{
    int oc = getTextColor();
    setTextColor(LIGHT_RED);

    printf("%s", info);

    setTextColor(oc);
}

void scm_throw_eval_error()
{
    longjmp(eval_error_jmp_buf, 1);
}

scm_object* scm_wrong_contract(const char *name, const char *expected, int index, int argc, scm_object *argv[])
{
    scm_print_error(name);
    scm_print_error(": contract violation;\n");
    scm_print_error("  expected: ");
    scm_print_error(expected);
    scm_print_error("\n");
    scm_print_error("  given: ");
    scm_write(scm_stdout_port, argv[index]);
    scm_print_error("\n");
    if (argc > 1) {
        scm_print_error("  argument position: ");
        char nstr[10] = {0};
        sprintf(nstr, "%d\n", index + 1);
        scm_print_error(nstr);

        scm_print_error("  other arguments...:\n");
        int argi;
        for (argi = 0; argi < argc; argi++) {
            if (argi == index)
                continue;
            scm_print_error("   ");
            scm_write(scm_stdout_port, argv[argi]);
            scm_print_error("\n");
        }
    }

    scm_throw_eval_error();

    return NULL;
}

scm_object* scm_mismatch_arity(scm_object *proc, int is_atleast, int expected_min, int expected_max, int argc, scm_object *argv[])
{
    const char *proc_name;
    if (SCM_COMPROCP(proc)) {
        proc_name = ((scm_compound_proc *)proc)->name;
    } else {
        proc_name = ((scm_primitive_proc *)proc)->name;
    }

    char expected[30];
    if(expected_max == -1)
        sprintf(expected, "%d", expected_min);
    else
        sprintf(expected, "%d to %d", expected_min, expected_max);

    scm_print_error((char*)proc_name);
    scm_print_error(": arity mismatch;\n");
    scm_print_error(" the expected number of arguments does not match the given number\n");
    scm_print_error("  expected: ");
    scm_print_error(is_atleast ? "at least " : "");
    scm_print_error(expected);
    scm_print_error("\n");
    scm_print_error("  given: ");
    char nstr[10] = {0};
    sprintf(nstr, "%d\n", argc);
    scm_print_error(nstr);
    if (argc > 0) {
        scm_print_error("  arguments...:\n");
        int argi;
        for (argi = 0; argi < argc; argi++) {
            scm_print_error("   ");
            scm_write(scm_stdout_port, argv[argi]);
            scm_print_error("\n");
        }
    }

    scm_throw_eval_error();

    return NULL;
}

scm_object* scm_undefined_identifier(scm_symbol *id)
{
    scm_print_error((char*)SCM_SYMBOL_STR_VAL(id));
    scm_print_error(": undefined;\n cannot reference undefined identifier\n");

    scm_throw_eval_error();

    return NULL;
}

scm_object* scm_out_of_range(const char *name, scm_object *obj, int start, int end, int isrange)
{
    char *type;
    size_t len;

    if (SCM_STRINGP(obj)) {
        type = "string";
        len = SCM_STR_LEN(obj);
    } else if (SCM_VECTORP(obj)) {
        type = "vector";
        len = SCM_VECTOR_LEN(obj);
    }

    char info[200] = {0};
    sprintf(info, "%s: ", type);
    if (len > 0) {
        char tmp[100] = {0};
        if (isrange) {
            if (!(0 <= start && start <= len)) {
                sprintf(tmp, "starting index is out of range\n"
                        "  starting index: %d\n", start);
            } else if (!(0 <= end && end <= len)) {
                sprintf(tmp, "ending index is out of range\n"
                        "  ending index: %d\n", end);
            } else if (start > end) {
                sprintf(tmp, "ending index is smaller than starting index\n"
                        "  ending index: %d\n  starting index: %d\n", end, start);
            }
        } else {
            sprintf(tmp, "index is out of range\n"
                    "  index: %d\n", start);
        }
        strcat(info, tmp);

        sprintf(tmp, "  valid range: [%d, %d]\n  %s: ",
            0, (int)(isrange ? len : len - 1), type);
        strcat(info ,tmp);
        scm_print_error(info);
        scm_write(scm_stdout_port, obj);
        scm_print_error("\n");
    } else {
        sprintf(info, "%s: index is out of range for empty %s\n"
            "  index: %d\n", name, type, start);
        scm_print_error(info);
    }

    scm_throw_eval_error();

    return NULL;
}
