#include "eval.h"
#include "env.h"
#include "bool.h"
#include "symbol.h"
#include "list.h"
#include "scm.h"


static scm_object* eval(scm_object *, scm_env *);
static scm_object* apply_primitive_procedure(scm_object *, int, scm_object *[]);
static scm_object* eval_definition(scm_object *, scm_env *);
static scm_object* eval_lambda(scm_object *, scm_env *);
static scm_env* make_apply_env(scm_compound_proc *, int, scm_object *[]);

scm_object* scm_eval(scm_object *exp, scm_env *env)
{
    return eval(exp, env);
}

static scm_object* eval(scm_object *exp, scm_env *env)
{
    EVAL:
    switch (SCM_TYPE(exp)) {
        case scm_true_type:
        case scm_false_type:
        case scm_integer_type:
        case scm_float_type:
        case scm_char_type:
        case scm_string_type:
        case scm_void_type:
            return exp;
        case scm_symbol_type:
            return scm_env_lookup(env, (scm_symbol *) exp);
        case scm_pair_type: {
            scm_symbol *operator = (scm_symbol *) scm_operator(exp);
            if (SCM_SYMBOLP(operator)) {
                if (SAME_OBJ(operator, scm_quote_symbol))
                    return scm_quoted_object(exp);
                if (SAME_OBJ(operator, scm_define_symbol))
                    return eval_definition(exp, env);
                if (SAME_OBJ(operator, scm_lambda_symbol))
                    return eval_lambda(exp, env);
                if (SAME_OBJ(operator, scm_begin_symbol)) {
                    scm_object *exps = scm_begin_actions(exp);
                    if(! SCM_NULLP(exps)) {
                        for(; ! SCM_NULLP( SCM_CDR(exps) ); exps = SCM_CDR(exps) )
                            eval(SCM_CAR(exps), env);
                        exp = SCM_CAR(exps);
                        goto EVAL;
                    } else {
                        return scm_void;
                    }
                }
                if (SAME_OBJ(operator, scm_if_symbol)) {
                    scm_object *optionalAlt;
                    // 首先求值谓词表达式
                    // 然后继续判断谓词的值：如果为真，返回后件，否则返回前件
                    exp = SCM_TRUEP( eval(scm_if_predicate(exp), env) ) ?
                          scm_if_consequent(exp) : (optionalAlt = scm_if_alternative(exp),
                                                    SCM_NULLP(optionalAlt) ? scm_void : optionalAlt);
                    goto EVAL;
                }
            }

            scm_object *proc = eval((scm_object *) operator, env);
            if (proc == NULL)
                return NULL;
            scm_object *operands = scm_operands(exp);
            //array of values
            //TODO: O(n)
            scm_object **argv = malloc(sizeof(scm_object *) * scm_list_length(operands));
            int argc = 0;
            while (!SCM_NULLP(operands)) {
                argv[argc++] = eval(SCM_CAR(operands), env);
                operands = SCM_CDR(operands);
            }

            if (SCM_PRIMPROCP(proc)) {
                return apply_primitive_procedure(proc, argc, argv);
            } else {
                exp = scm_make_begin(((scm_compound_proc *)proc)->body);
                env = make_apply_env((scm_compound_proc *)proc, argc, argv);
                goto EVAL;
            }
            return exp;
            break;
        }
        case scm_null_type:
            break;
        default:;
    }
}

static scm_object* apply_primitive_procedure(scm_object *proc, int argc, scm_object *argv[])
{
    return ((scm_primitive_proc *)proc)->prim(argc, argv);
}

static scm_object* eval_lambda(scm_object *exp, scm_env *env)
{
    // make compound procedure
    scm_compound_proc *proc = (scm_compound_proc *)scm_malloc_object(sizeof(scm_compound_proc));
    ((scm_object *)proc)->type = scm_compound_type;
    proc->name = "";
    proc->body = scm_lambda_body(exp);
    proc->env = env;

    scm_object *formals = scm_lambda_paramters(exp);

    proc->params = formals;

    //计算参数数量
    if(SCM_PAIRP(formals)) {
        int len = scm_list_length(formals);
        proc->min_arity = len;
        if(SCM_LISTP(formals)) {
            proc->max_arity = len;
        } else {
            proc->max_arity = -1;
        }
    } else if(SCM_SYMBOLP(formals)) {
        proc->min_arity = 0;
        proc->max_arity = -1;
    } else {
        // 'not an identifier'
    }

    return (scm_object *)proc;
}

static scm_object* eval_definition(scm_object *exp, scm_env *env)
{
    scm_object *id = scm_definition_var(exp);
    scm_object *val = eval(scm_definition_val(exp), env);
    if(SCM_COMPROCP(val))
        ((scm_compound_proc *)val)->name = SCM_SYMBOL_STR_VAL(id);
    scm_env_add_binding(env, (scm_symbol *)id, val);
    return scm_void;
}

static scm_env* make_apply_env(scm_compound_proc *proc, int argc, scm_object *argv[])
{
    return proc->env;
}

scm_object* scm_definition_var(scm_object *exp)
{
    if(SCM_SYMBOLP(SCM_CADR(exp)))
        return SCM_CADR(exp);
    else if(SCM_PAIRP(SCM_CADR(exp)))
        return SCM_CAADR(exp);
    else
        return SCM_CADR(exp);
}

scm_object* scm_definition_val(scm_object *exp)
{
    if(SCM_SYMBOLP(SCM_CADR(exp)))
        return SCM_CADDR(exp);
    else {
        return scm_make_lambda(SCM_CDADR(exp), SCM_CDDR(exp));
    }
}

scm_object* scm_make_lambda(scm_object *parameters, scm_object *body)
{
    return SCM_CONS((scm_object *)scm_lambda_symbol, SCM_CONS(parameters, body));
}

