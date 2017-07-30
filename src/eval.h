#ifndef SCHEME_EVAL_H
#define SCHEME_EVAL_H

#include "scm.h"
#include <setjmp.h>

#define scm_operator SCM_CAR
#define scm_operands SCM_CDR
#define scm_quoted_object SCM_CADR
#define scm_if_predicate SCM_CADR
#define scm_if_consequent SCM_CADDR
#define scm_if_alternative(exp) (SCM_NULLP(SCM_CDDDR(exp)) ? SCM_CDDDR(exp) : SCM_CADDDR(exp))
#define scm_lambda_paramters SCM_CADR
#define scm_lambda_body SCM_CDDR
#define scm_begin_actions SCM_CDR
#define scm_make_begin(seq) SCM_CONS((scm_object *)scm_begin_symbol, seq)

extern  jmp_buf eval_error_jmp;

scm_object* scm_definition_var(scm_object *);
scm_object* scm_definition_val(scm_object *);
scm_object* scm_make_lambda(scm_object *, scm_object *);

void scm_init();
scm_object* scm_eval(scm_object *);

#endif //SCHEME_EVAL_H
