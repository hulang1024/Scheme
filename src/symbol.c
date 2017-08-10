#include <string.h>
#include "symbol.h"
#include "bool.h"
#include "list.h"
#include "env.h"
#include "error.h"
#include "hashtable.h"

// TODO: hashtable
static hashtable *symbols = NULL;
static int gen_sym_id = 0;

static scm_object* symbol_p_prim(int, scm_object *[]);

static int symbol_equal(void *x, void *y)
{
    return strcmp((char *)x, (char *)y) == 0;
}

static int symbol_hash(void *s)
{
    int ret = 0;
    char *sc = s;
    while(*sc) {
        ret *= 10;
        ret += *sc++;
    }

    if (ret < 0) ret = -ret;

    return ret;
}

void scm_init_symbol(scm_env *env)
{
    symbols = hashtable_new(100, symbol_equal, symbol_hash);

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
    scm_not_symbol = scm_get_intern_symbol("not");
    scm_and_symbol = scm_get_intern_symbol("and");
    scm_or_symbol = scm_get_intern_symbol("or");
    scm_when_symbol = scm_get_intern_symbol("when");
    scm_unless_symbol = scm_get_intern_symbol("unless");
    scm_let_symbol = scm_get_intern_symbol("let");
    scm_do_symbol = scm_get_intern_symbol("do");
    scm_while_symbol = scm_get_intern_symbol("while");
    scm_for_symbol = scm_get_intern_symbol("for");


    scm_plus_symbol = scm_get_intern_symbol("+");
    scm_minus_symbol = scm_get_intern_symbol("-");
    scm_mul_symbol = scm_get_intern_symbol("*");
    scm_div_symbol = scm_get_intern_symbol("/");
    scm_lt_symbol = scm_get_intern_symbol("<");
    
    scm_memv_symbol = scm_get_intern_symbol("memv");

    scm_add_prim(env, "symbol?", symbol_p_prim, 1, 1);
}

static scm_object* scm_make_symbol(const char *s)
{
    scm_object *o = scm_malloc_object(sizeof(scm_symbol));
    o->type = scm_symbol_type;
    SCM_SYMBOL_STR_VAL(o) = s;
    return o;
}

scm_symbol* scm_get_intern_symbol(const char *str)
{
    assert(symbols != NULL);

    scm_symbol *sym = hashtable_get(symbols, (void *)str);

    if (sym == NULL) { // if not interned
        sym = (scm_symbol *) scm_make_symbol((char *)str);
        // intern
        hashtable_set(symbols, (void *)str, sym);
    }

    return sym;
}

void scm_reset_gen_symbol()
{
    gen_sym_id = 0;
}

scm_object* scm_gen_symbol()
{
    gen_sym_id++;
    return scm_make_symbol((const char *)gen_sym_id);
}

static scm_object* symbol_p_prim(int argc, scm_object *argv[])
{
    return SCM_BOOL(SCM_SYMBOLP(argv[0]));
}