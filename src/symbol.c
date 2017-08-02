#include <string.h>
#include "symbol.h"
#include "bool.h"
#include "list.h"
#include "env.h"
#include "error.h"

// TODO: hashtable
static scm_object *symbols = scm_null;
static int gen_sym_id = 0;

static scm_object* symbol_p_prim(int, scm_object *[]);

void scm_init_symbol(scm_env *env)
{
    scm_quote_symbol = scm_get_intern_symbol("quote");
    scm_dot_symbol = scm_get_intern_symbol(".");
    scm_if_symbol = scm_get_intern_symbol("if");
    scm_define_symbol = scm_get_intern_symbol("define");
    scm_assignment_symbol = scm_get_intern_symbol("set!");
    scm_lambda_symbol = scm_get_intern_symbol("lambda");
    scm_begin_symbol = scm_get_intern_symbol("begin");
    scm_cond_symbol = scm_get_intern_symbol("cond");
    scm_case_symbol = scm_get_intern_symbol("case");
    scm_else_symbol = scm_get_intern_symbol("else");
    scm_and_symbol = scm_get_intern_symbol("and");
    scm_or_symbol = scm_get_intern_symbol("or");
    scm_when_symbol = scm_get_intern_symbol("when");
    scm_unless_symbol = scm_get_intern_symbol("unless");
    scm_let_symbol = scm_get_intern_symbol("let");
    scm_do_symbol = scm_get_intern_symbol("do");
    scm_while_symbol = scm_get_intern_symbol("while");
    scm_for_symbol = scm_get_intern_symbol("for");
    
    scm_add_prim(env, "symbol?", symbol_p_prim, 1, 1);
}

scm_object* scm_make_symbol(const char *s)
{
    scm_object *o = scm_malloc_object(sizeof(scm_symbol));
    o->type = scm_symbol_type;
    SCM_SYMBOL_STR_VAL(o) = s;
    return o;
}

static void intern_symbol(scm_symbol *sym)
{
    symbols = cons((scm_object *)sym, symbols);
}

scm_symbol* scm_get_intern_symbol(const char *str)
{
    scm_object *syms = symbols;
    scm_symbol *sym;
    while (!SCM_NULLP(syms)) {
        sym = (scm_symbol *)SCM_CAR(syms);
        if (stricmp(SCM_SYMBOL_STR_VAL(sym), str) == 0)
            return sym;
        syms = SCM_CDR(syms);
    }
    // if not interned
    sym = (scm_symbol *)scm_make_symbol(str);
    intern_symbol(sym);
    return sym;
}

void scm_reset_gen_symbol()
{
    gen_sym_id = 0;
}

scm_object* scm_gen_symbol()
{
    gen_sym_id++;
    scm_object *sym = scm_make_symbol(NULL);
    intern_symbol((scm_symbol *)sym);
    return sym;
}

static scm_object* symbol_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_SYMBOLP(argv[0]));
}