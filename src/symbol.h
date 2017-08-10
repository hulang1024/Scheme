#ifndef SCHEME_SYMBOL_H
#define SCHEME_SYMBOL_H

#include "scm.h"

scm_symbol *scm_quote_symbol;
scm_symbol *scm_dot_symbol;
scm_symbol *scm_if_symbol;
scm_symbol *scm_define_symbol;
scm_symbol *scm_assignment_symbol;
scm_symbol *scm_lambda_symbol;
scm_symbol *scm_begin_symbol;

scm_symbol *scm_cond_symbol;
scm_symbol *scm_case_symbol;
scm_symbol *scm_else_symbol;
scm_symbol *scm_not_symbol;
scm_symbol *scm_and_symbol;
scm_symbol *scm_or_symbol;
scm_symbol *scm_when_symbol;
scm_symbol *scm_unless_symbol;

scm_symbol *scm_let_symbol;

scm_symbol *scm_do_symbol;
scm_symbol *scm_while_symbol;
scm_symbol *scm_for_symbol;

scm_symbol *scm_plus_symbol;
scm_symbol *scm_minus_symbol;
scm_symbol *scm_mul_symbol;
scm_symbol *scm_div_symbol;
scm_symbol *scm_lt_symbol;

scm_symbol *scm_memv_symbol;


void scm_init_symbol(scm_env *);
scm_symbol* scm_get_intern_symbol(const char *);
scm_object* scm_gen_symbol();

#endif //SCHEME_SYMBOL_H
