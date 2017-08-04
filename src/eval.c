#include "eval.h"
#include "env.h"
#include "bool.h"
#include "symbol.h"
#include "list.h"
#include "error.h"
#include "port.h"
#include "print.h"
#include "scm.h"

#define EVAL(expr) { exp = expr; goto EVAL; }

#define apply_primitive_procedure(proc, argc, argv) (((scm_primitive_proc *)proc)->prim(argc, argv))
#define when_to_if(exp) \
    scm_make_if1(scm_when_test(exp), scm_make_begin(scm_when_body(exp)))
#define unless_to_if(exp) \
    scm_make_if1( \
        scm_make_app(scm_not_symbol, SCM_LIST1(scm_unless_test(exp))), \
        scm_make_begin(scm_unless_body(exp)))

static scm_object* eval_prim(int, scm_object *[]);
static scm_object* eval(scm_object *, scm_env *);
static scm_object* eval_definition(scm_object *, scm_env *);
static scm_object* eval_assignment(scm_object *, scm_env *);
static scm_object* eval_lambda(scm_object *, scm_env *);
static scm_env* make_apply_env(scm_compound_proc *, int, scm_object *[]);
static int match_arity(scm_object *, int, scm_object *[]);

static scm_object* let_to_combination(scm_object *);
static scm_object* and_to_if(scm_object *);
static scm_object* or_to_if(scm_object *);

static scm_env *global_env;

jmp_buf eval_error_jmp_buf;


void scm_init()
{
    global_env = scm_basic_env();
}

void scm_init_eval(scm_env *env)
{
    scm_add_prim(env, "eval", eval_prim, 1, 2);
}

scm_object* scm_eval(scm_object *exp)
{
    if (setjmp(eval_error_jmp_buf) == 1) {
        return NULL;
    }
    return eval(exp, global_env);
}

scm_object* scm_apply(scm_object *proc, int argc, scm_object *argv[])
{
    if (SCM_PRIMPROCP(proc)) {
        if (match_arity(proc, argc, argv))
            return apply_primitive_procedure(proc, argc, argv);
    } else if (SCM_COMPROCP(proc)) {
        if (match_arity(proc, argc, argv))
            return eval(scm_make_begin(((scm_compound_proc *)proc)->body),
                make_apply_env((scm_compound_proc *)proc, argc, argv));
    }
    return NULL;
}

static scm_object* eval_prim(int argc, scm_object *argv[])
{
    if (argc == 1) {
        return eval(argv[0], global_env);
    } else {
        if (!SCM_NAMESPACEP(argv[1]))
            return scm_wrong_contract("eval", "namespace?", 1, argc, argv);
        return eval(argv[0], (scm_env *)argv[1]);
    }
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
            
        case scm_symbol_type: {
            scm_env_entry *entry = scm_env_lookup(env, (scm_symbol *) exp);
            if (entry != NULL) {
                return entry->val;
            } else {
                return scm_undefined_identifier((scm_symbol *)exp);
            }
        }
        
        case scm_pair_type: {
            if (!SCM_LISTP(exp)) {
                scm_print_error("application: bad syntax\n");
                scm_print_error("  in: ");
                scm_write(scm_stdout_port, exp);
                scm_print_error("\n");
                scm_throw_error();
                return NULL;
            }
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
                    if (! SCM_NULLP(exps)) {
                        for(; ! SCM_NULLP( SCM_CDR(exps) ); exps = SCM_CDR(exps) )
                            eval(SCM_CAR(exps), env);
                        EVAL(SCM_CAR(exps));
                    } else {
                        return scm_void;
                    }
                }
                if (SAME_OBJ(operator, scm_if_symbol)) {
                    scm_object *optionalAlt;
                    EVAL(SCM_TRUEP( eval(scm_if_predicate(exp), env) ) ?
                          scm_if_consequent(exp) : (optionalAlt = scm_if_alternative(exp),
                                                    SCM_NULLP(optionalAlt) ? scm_void : optionalAlt));
                }
                if (SAME_OBJ(operator, scm_let_symbol)) {
                    EVAL(let_to_combination(exp));
                }
                if (SAME_OBJ(operator, scm_and_symbol)) {
                    EVAL(and_to_if(exp));
                }
                if (SAME_OBJ(operator, scm_or_symbol)) {
                    EVAL(or_to_if(exp));
                }
                if (SAME_OBJ(operator, scm_when_symbol)) {
                    EVAL(when_to_if(exp));
                }
                if (SAME_OBJ(operator, scm_unless_symbol)) {
                    EVAL(unless_to_if(exp));
                }
                if (SAME_OBJ(operator, scm_assignment_symbol))
                    return eval_assignment(exp, env);
            }

            scm_object *proc = eval((scm_object *) operator, env);
            scm_object *operands = scm_operands(exp);
            //array of values
            //TODO: O(n)
            scm_object **argv = malloc(sizeof(scm_object *) * scm_list_length(operands));
            int argc = 0;
            scm_list_for_each(operands) {
                argv[argc++] = eval(SCM_CAR(operands), env);
            }

            if (SCM_PRIMPROCP(proc)) {
                if (match_arity(proc, argc, argv))
                    return apply_primitive_procedure(proc, argc, argv);
            } else if (SCM_COMPROCP(proc)) {
                if (match_arity(proc, argc, argv)) {
                    exp = scm_make_begin(((scm_compound_proc *)proc)->body);
                    env = make_apply_env((scm_compound_proc *)proc, argc, argv);
                    goto EVAL;
                }
            } else {
                scm_print_error("application: not a procedure;\n"\
                    " expected a procedure that can be applied to arguments\n");
                scm_throw_error();
            }
            break;
        }
        case scm_null_type:
            break;
        default: ;
    }
    return NULL;
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

    if (SCM_PAIRP(formals)) {
        int len = scm_list_length(formals);
        int param_i = 0;
        proc->params = malloc(len * sizeof(scm_object *));
        proc->min_arity = len;

        while (SCM_PAIRP(formals)) {
            proc->params[param_i++] = SCM_CAR(formals);
            formals = SCM_CDR(formals);
        }
        if (SCM_NULLP(formals)) {
            proc->max_arity = len;
        } else {
            proc->params[param_i++] = formals;
            proc->max_arity = -1;
        }
        proc->params_len = param_i;
    } else if (SCM_NULLP(formals)) {
        proc->min_arity = 0;
        proc->max_arity = 0;
        proc->params_len = 0;
    } else if (SCM_SYMBOLP(formals)) {
        proc->params = malloc(sizeof(scm_object *));
        proc->params[0] = formals;
        proc->params_len = 1;
        proc->min_arity = 0;
        proc->max_arity = -1;
    } else {
        scm_print_error("lambda: argument sequence\n");
        scm_print_error("  in: ");
        scm_write(scm_stdout_port, exp);
        scm_print_error("\n");
        scm_throw_error();
    }

    return (scm_object *)proc;
}

static scm_object* eval_definition(scm_object *exp, scm_env *env)
{
    scm_symbol *id = scm_definition_var(exp);
    scm_object *val = eval(scm_definition_val(exp), env);
    if (SCM_COMPROCP(val))
        ((scm_compound_proc *)val)->name = SCM_SYMBOL_STR_VAL(id);
    
    scm_env_entry * entry = scm_env_lookup(env, id);
    if (entry == NULL) {
        scm_env_add_binding(env, id, val);
    } else {
        entry->val = val;
    }
    
    return scm_void;
}

static scm_object* eval_assignment(scm_object *exp, scm_env *env)
{
    scm_symbol *id = scm_assignment_var(exp);
    scm_env_entry * entry = scm_env_lookup(env, id);
    if (entry != NULL) {
        entry->val = eval(scm_assignment_val(exp), env);
    } else {
        // if entry is NULL
        scm_undefined_identifier(id);
    }
    return scm_void;
}

static scm_env* make_apply_env(scm_compound_proc *proc, int argc, scm_object *argv[])
{
    if (proc->params_len > 0) {
        scm_env *apply_env = (scm_env *)scm_malloc_object(sizeof(scm_env));
        apply_env->rest = NULL;

        int index;
        if (proc->min_arity == proc->max_arity) { // '(x y z)
            for (index = 0; index < proc->params_len; index++)
                scm_env_add_binding(apply_env, (scm_symbol *)proc->params[index], argv[index]);
        } else {
            if (proc->min_arity > 0) { // '(x y . z)
                for (index = 0; index < proc->params_len - 1; index++)
                    scm_env_add_binding(apply_env, (scm_symbol *) proc->params[index], argv[index]);
                scm_env_add_binding(apply_env, (scm_symbol *)proc->params[index],
                    scm_build_list(argc - index, argv + index));
            } else { // 'x
                scm_env_add_binding(apply_env, (scm_symbol *)proc->params[0], scm_build_list(argc, argv));
            }
        }
        // TODO: high efficient!
        scm_env *base_env = apply_env;
        while (base_env->rest)
            base_env = base_env->rest;
        base_env->rest = proc->env;

        return apply_env;
    } else { // '()
        return proc->env;
    }
}

static int match_arity(scm_object *proc, int argc, scm_object *argv[])
{
    int min , max;

    if (SCM_COMPROCP(proc)) {
        min = ((scm_compound_proc *)proc)->min_arity;
        max = ((scm_compound_proc *)proc)->max_arity;
    } else {
        min = ((scm_primitive_proc *)proc)->min_arity;
        max = ((scm_primitive_proc *)proc)->max_arity;
    }

    if (min == max) {
        if (argc != min)
            scm_mismatch_arity(proc, 0, min, -1, argc, argv);
    } else {
        if (max > 0) {
            if (argc < min || argc > max)
                scm_mismatch_arity(proc, 0, min, max, argc, argv);
        } else {
            max = 0x3FFFFFFE;
            if (argc < min || argc > max)
                scm_mismatch_arity(proc, 1, min, -1, argc, argv);
        }
    }

    return 1;
}


/* built-in syntax transformers */
static scm_object* let_to_combination(scm_object *exp)
{
    scm_object *bindings = scm_let_bindings(exp);
    scm_object *body = scm_let_body(exp);
    scm_object *let_binding_vars = scm_null, *var_last;
    scm_object *let_binding_inits = scm_null, *init_last;

    scm_list_for_each(bindings) {
        if(SCM_NULLP(let_binding_vars)) {
            let_binding_vars = var_last = SCM_LCONS(SCM_CAAR(bindings), scm_null);
            let_binding_inits = init_last = SCM_LCONS(SCM_CADAR(bindings), scm_null);
        } else {
            var_last = SCM_CDR(var_last) = SCM_LCONS(SCM_CAAR(bindings), scm_null);
            init_last = SCM_CDR(init_last) = SCM_LCONS(SCM_CADAR(bindings), scm_null);
        }
    }

    if(scm_is_named_let(exp)) {
        return scm_make_app0(
                   scm_make_lambda(
                       scm_null,
                       SCM_LIST2(
                           scm_make_def(scm_let_var(exp), scm_make_lambda(let_binding_vars, body)),
                           scm_make_app(scm_let_var(exp), let_binding_inits))));
    }
    else {
        return scm_make_app(
                   scm_make_lambda(let_binding_vars, body),
                   let_binding_inits);
    }
}

#define GEN_AND_OR_OR(name, val_ifnull, pred_exp) \
    static scm_object* name(scm_object *exp) \
    { \
        if(SCM_NULLP(SCM_CDR(exp))) \
            return val_ifnull; \
        \
        scm_object *temp_var = scm_gen_symbol(); \
        scm_object *head = NULL, *prev; \
        scm_object *let_exp, *if_exp; \
        \
        exp = SCM_CDR(exp); \
        scm_list_for_each(exp) { \
            if(!SCM_NULLP(SCM_CDR(exp))) { \
                if_exp = scm_make_if(pred_exp, temp_var, NULL); \
                let_exp = scm_make_let(SCM_LIST1(SCM_LIST2(temp_var, SCM_CAR(exp))), if_exp); \
                if (head != NULL) \
                    SCM_CDR(prev) = SCM_LIST1(let_exp); \
                else \
                    head = let_exp; \
                prev = SCM_CDDR(if_exp); \
            } else { \
                if (head != NULL) \
                    SCM_CDR(prev) = SCM_LIST1(SCM_CAR(exp)); \
                else { \
                    head = SCM_CAR(exp); \
                } \
            } \
            \
        } \
        \
        return head; \
    }

GEN_AND_OR_OR(and_to_if, scm_true, scm_make_app(scm_not_symbol, SCM_LIST1(temp_var)));
GEN_AND_OR_OR(or_to_if, scm_false, temp_var);
