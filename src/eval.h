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
#define scm_make_lambda(params, body) SCM_CONS((scm_object *)scm_lambda_symbol, SCM_CONS(params, body))

#define scm_begin_actions SCM_CDR
#define scm_make_begin(seq) SCM_CONS((scm_object *)scm_begin_symbol, seq)

#define scm_definition_var(exp) ((scm_symbol *)(SCM_SYMBOLP(SCM_CADR(exp)) ? \
    SCM_CADR(exp) : SCM_PAIRP(SCM_CADR(exp)) ? SCM_CAADR(exp) : SCM_CADR(exp)))
    
#define scm_definition_val(exp) (SCM_SYMBOLP(SCM_CADR(exp)) ? \
    SCM_CADDR(exp) : scm_make_lambda(SCM_CDADR(exp), SCM_CDDR(exp)))

#define scm_assignment_var(exp) ((scm_symbol *)SCM_CADR(exp))
#define scm_assignment_val SCM_CADDR


extern jmp_buf eval_error_jmp_buf;

void scm_init();
void scm_init_eval(scm_env *);
scm_object* scm_eval(scm_object *);
scm_object* scm_apply(scm_object *, int, scm_object *[]);

#endif //SCHEME_EVAL_H
