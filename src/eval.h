#ifndef SCHEME_EVAL_H
#define SCHEME_EVAL_H

#include "scm.h"
#include "list.h"
#include <setjmp.h>


#define scm_operator SCM_CAR
#define scm_operands SCM_CDR
#define scm_make_app(operator, operands) SCM_LCONS((scm_object *)operator, operands)
#define scm_make_app0(operator) scm_make_app(operator, scm_null)

#define scm_quoted_object SCM_CADR
#define scm_make_quotation(o) SCM_LIST2((scm_object *)scm_quote_symbol, o)

#define scm_if_predicate SCM_CADR
#define scm_if_consequent SCM_CADDR
#define scm_if_alternative(exp) (SCM_NULLP(SCM_CDDDR(exp)) ? SCM_CDDDR(exp) : SCM_CADDDR(exp))
#define scm_make_if(pred, consequent, alternative) \
    SCM_LIST4((scm_object *)scm_if_symbol, pred, consequent, alternative)
#define scm_make_if1(pred, consequent) SCM_LIST3((scm_object *)scm_if_symbol, pred, consequent)

#define scm_lambda_paramters SCM_CADR
#define scm_lambda_body SCM_CDDR
#define scm_make_lambda(params, body) SCM_LCONS((scm_object *)scm_lambda_symbol, SCM_CONS(params, body))

#define scm_definition_var(exp) ((scm_symbol *)(SCM_SYMBOLP(SCM_CADR(exp)) ? \
    SCM_CADR(exp) : SCM_PAIRP(SCM_CADR(exp)) ? \
        SCM_CAADR(exp) : SCM_CADR(exp)))
#define scm_definition_val(exp) (SCM_SYMBOLP(SCM_CADR(exp)) ? \
    SCM_CADDR(exp) : scm_make_lambda(SCM_CDADR(exp), SCM_CDDR(exp)))
#define scm_make_def(var, val) SCM_LIST3((scm_object *)scm_define_symbol, var, val)

#define scm_assignment_var(exp) ((scm_symbol *)SCM_CADR(exp))
#define scm_assignment_val SCM_CADDR

#define scm_begin_actions SCM_CDR
#define scm_make_begin(seq) SCM_LCONS((scm_object *)scm_begin_symbol, seq)
#define scm_sequence_exp(seq) (SCM_NULLP(seq) ? \
    seq : SCM_NULLP(SCM_CDR(seq)) ? SCM_CDR(seq) : scm_make_begin(seq))


#define scm_and_tests SCM_CDR
#define scm_or_tests SCM_CDR

#define scm_when_test SCM_CADR
#define scm_when_body SCM_CDDR
#define scm_unless_test SCM_CADR
#define scm_unless_body SCM_CDDR

#define scm_is_named_let(exp) SCM_SYMBOLP(SCM_CADR(exp))
#define scm_let_var(exp) SCM_CADR(exp)
#define scm_let_bindings(exp) (scm_is_named_let(exp) ? SCM_CADDR(exp) : SCM_CADR(exp))
#define scm_let_body(exp) (scm_is_named_let(exp) ? SCM_CDDDR(exp) : SCM_CDDR(exp))
#define scm_make_let(bindings, body) SCM_LIST3((scm_object *)scm_let_symbol, bindings, body)
#define scm_make_named_let(name, bindings, body) SCM_LIST4((scm_object *)scm_let_symbol, name, bindings, body)


extern jmp_buf eval_error_jmp_buf;

void scm_init();
void scm_init_eval(scm_env *);
scm_object* scm_eval(scm_object *);
scm_object* scm_apply(scm_object *, int, scm_object *[]);

#endif //SCHEME_EVAL_H
