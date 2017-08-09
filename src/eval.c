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
static scm_object* cond_to_if(scm_object *);
static scm_object* case_to_cond(scm_object *);
static scm_object* do_to_more_prim(scm_object *);
static scm_object* while_to_more_prim(scm_object *);
static scm_object* for_to_more_prim(scm_object *);

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
    // 根据表达式类型dispatch动作
    switch (SCM_TYPE(exp)) {
        case scm_true_type:
        case scm_false_type:
        case scm_integer_type:
        case scm_float_type:
        case scm_char_type:
        case scm_string_type:
        case scm_void_type:
            // self evaluating
            return exp;
            
        case scm_symbol_type: {
            // 是符号，查询在环境中关联的值
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
                scm_throw_eval_error();
                return NULL;
            }
            scm_symbol *operator = (scm_symbol *) scm_operator(exp);
            // 如果运算符为符号，可能是语法关键字
            if (SCM_SYMBOLP(operator)) {
                if (SAME_OBJ(operator, scm_quote_symbol))
                    // eval quotation
                    return scm_quoted_object(exp);
                if (SAME_OBJ(operator, scm_define_symbol))
                    return eval_definition(exp, env);
                if (SAME_OBJ(operator, scm_lambda_symbol))
                    return eval_lambda(exp, env);
                if (SAME_OBJ(operator, scm_begin_symbol)) {
                    /* 求值顺序表达式/序列 */
                    scm_object *exps = scm_begin_actions(exp);
                    if (! SCM_NULLP(exps)) {
                        // 顺序求值尾部前面的表达式
                        for(; ! SCM_NULLP( SCM_CDR(exps) ); exps = SCM_CDR(exps) )
                            eval(SCM_CAR(exps), env);
                        // 迭代求值尾上下文中的表达式
                        EVAL(SCM_CAR(exps));
                    } else {
                        return scm_void;
                    }
                }
                if (SAME_OBJ(operator, scm_if_symbol)) {
                    /* 求值if表达式 */
                    scm_object *optionalAlt;
                    // 首先求值谓词表达式
                    // 然后继续判断谓词的值：如果为真，返回迭代求值后件，否则迭代求值前件
                    EVAL(SCM_TRUEP( eval(scm_if_predicate(exp), env) ) ?
                          scm_if_consequent(exp) : (optionalAlt = scm_if_alternative(exp),
                                                    SCM_NULLP(optionalAlt) ? scm_void : optionalAlt));
                }
                if (SAME_OBJ(operator, scm_let_symbol)) {
                    /* let在语法上变换到lambda */
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
                if (SAME_OBJ(operator, scm_cond_symbol)) {
                    EVAL(cond_to_if(exp));
                }
                if (SAME_OBJ(operator, scm_case_symbol)) {
                    EVAL(case_to_cond(exp));
                }
                if (SAME_OBJ(operator, scm_do_symbol)) {
                    EVAL(do_to_more_prim(exp));
                }
                if (SAME_OBJ(operator, scm_while_symbol)) {
                    EVAL(while_to_more_prim(exp));
                }
                if (SAME_OBJ(operator, scm_for_symbol)) {
                    EVAL(for_to_more_prim(exp));
                }
                if (SAME_OBJ(operator, scm_assignment_symbol))
                    return eval_assignment(exp, env);
            }

            /* 另外，是符号但不是语法关键字，或者不是符号，就是过程调用表达式：*/
            // 首先求值运算符，得到过程对象
            scm_object *proc = eval((scm_object *) operator, env);
            scm_object *operands = scm_operands(exp);

            // 然后求值运算数，得到实际参数
            //array of values
            //TODO: O(n)
            scm_object **argv = malloc(sizeof(scm_object *) * scm_list_length(operands));
            int argc = 0;
            scm_list_for_each(operands) {
                argv[argc++] = eval(SCM_CAR(operands), env);
            }

            if (SCM_PRIMPROCP(proc)) {
                // 检查实参个数是否匹配形参个数
                if (match_arity(proc, argc, argv))
                    return apply_primitive_procedure(proc, argc, argv);
            } else if (SCM_COMPROCP(proc)) {
                if (match_arity(proc, argc, argv)) {
                    // 将过程体转换为begin类型表达式
                    exp = scm_make_begin(((scm_compound_proc *)proc)->body);
                    // 构造一个用于执行过程调用的新环境
                    env = make_apply_env((scm_compound_proc *)proc, argc, argv);
                    // 在新环境上下文中迭代求值过程体。注意这里没有去递归调用eval，上同
                    goto EVAL;
                }
            } else {
                scm_print_error("application: not a procedure;\n"\
                    " expected a procedure that can be applied to arguments\n");
                scm_throw_eval_error();
            }
            break;
        }
        case scm_null_type:
            scm_print_error("application: illegal empty application;\n");
            scm_throw_eval_error();
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
        scm_throw_eval_error();
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
        // 将创建该过程时的环境作为外围环境
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
            if (argc < min)
                scm_mismatch_arity(proc, 1, min, -1, argc, argv);
        }
    }

    return 1;
}


/*    built-in syntax transformers    */

static scm_object* let_to_combination(scm_object *exp)
{
    // pluck bindings vars, inits
    scm_object *bindings = scm_let_bindings(exp);
    scm_pair binding_vars_head;
    scm_object *var_prev = &binding_vars_head;
    scm_pair binding_inits_head;
    scm_object *init_prev = &binding_inits_head;
    scm_list_for_each(bindings) {
        var_prev = SCM_CDR(var_prev) = SCM_LCONS(SCM_CAAR(bindings), scm_null);
        init_prev = SCM_CDR(init_prev) = SCM_LCONS(SCM_CADAR(bindings), scm_null);
    }
    
    scm_object *binding_vars = var_prev != binding_vars_head ? SCM_CDR(&binding_vars_head) : scm_null;
    scm_object *binding_inits = init_prev != binding_inits_head ? SCM_CDR(&binding_inits_head) : scm_null;
    
    scm_object *body = scm_let_body(exp);
    
    if(scm_is_named_let(exp)) {
        return scm_make_app0(
                   scm_make_lambda(
                       scm_null,
                       SCM_LIST2(
                           scm_make_def(scm_let_var(exp), scm_make_lambda(binding_vars, body)),
                           scm_make_app(scm_let_var(exp), binding_inits))));
    } else {
        return scm_make_app(
                   scm_make_lambda(binding_vars, body),
                   binding_inits);
    }
}

#define GEN_AND_OR_OR_TRANS(name, val_ifnull, pred_exp) \
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
            if (!SCM_NULLP(SCM_CDR(exp))) { \
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

GEN_AND_OR_OR_TRANS(and_to_if, scm_true, scm_make_app(scm_not_symbol, SCM_LIST1(temp_var)));
GEN_AND_OR_OR_TRANS(or_to_if, scm_false, temp_var);

static scm_object* cond_to_if(scm_object *exp)
{
    #define it_clause_actions(exp) scm_sequence_exp(scm_clause_actions(SCM_CAR(exp)))

    if(SCM_NULLP(scm_cond_clauses(exp)))
        return scm_void;
    
    scm_object *head = NULL, *prev;
    scm_object *if_exp;
    
    // for each clauses
    exp = scm_cond_clauses(exp);
    scm_list_for_each(exp) {
        if (!scm_is_else_clause(SCM_CAR(exp))) {
            if_exp = scm_make_if(scm_clause_test(SCM_CAR(exp)), it_clause_actions(exp), scm_void);
            if (head != NULL)
                SCM_CDR(prev) = SCM_LIST1(if_exp);
            else
                head = if_exp;
            prev = SCM_CDDR(if_exp);
        } else {
            if (!SCM_NULLP(SCM_CDR(exp))) {
                scm_print_error("cond: bad syntax (`else' clause must be last) ");
                scm_print_error("  in: ");
                scm_write(scm_stdout_port, SCM_CAR(exp));
                scm_print_error("\n");
                scm_throw_eval_error();
            }
            if (head != NULL)
                SCM_CDR(prev) = SCM_LIST1(it_clause_actions(exp));
            else {
                head = it_clause_actions(exp);
            }
        }
    }
    
    return head;
}

static scm_object* case_to_cond(scm_object *exp)
{
    scm_object *cond_clauses = scm_null, *last;
    scm_object *temp_var = scm_gen_symbol();
    // map clauses
    scm_object *clauses = scm_case_clauses(exp);
    scm_list_for_each(clauses) {
        if (!scm_is_else_clause(SCM_CAR(clauses))) {
            cond_clauses = last = scm_make_app(
                                      scm_memv_symbol,
                                      SCM_LIST2(temp_var,
                                                scm_make_quotation(scm_clause_test(SCM_CAR(clauses)))));
        } else {
            last = SCM_CDR(last) = SCM_CAR(clauses);
        }
    }
    
    return scm_make_let(
               SCM_LIST1(SCM_LIST2(temp_var, scm_case_key(exp))),
               scm_make_cond(cond_clauses);
}

static scm_object* do_to_more_prim(scm_object *exp)
{
    scm_object *bindings = do_bindings(exp);
    scm_pair let_bindings_head;
    scm_object *bind_prev = &let_bindings_head;
    scm_pair steps_head;
    scm_object *step_prev = &steps_head;
    // pluck bindings (var, init), steps
    scm_list_for_each(bindings) {
        bind_prev = SCM_CDR(bind_prev) = SCM_LIST2(SCM_CAAR(bindings), SCM_CADAR(bindings));
        step_prev = SCM_CDR(step_prev) = SCM_CADDAR(bindings);
    }
    scm_object *let_bindings = bind_prev != let_bindings_head ? SCM_CDR(&let_bindings_head) : scm_null;
    scm_object *steps = step_prev != steps_head ? SCM_CDR(&steps_head) : scm_null;
    
    scm_object *var = scm_gen_symbol();
    scm_object *alt = scm_do_commands(exp);
    *(SCM_NULLP(alt) ? &alt : &SCM_CDR(alt)) = SCM_LIST1(scm_make_app(var, steps));

    return scm_make_named_let(
               var,
               let_bindings,
               scm_make_if(
                   scm_do_test(exp),
                   scm_sequence_exp(scm_do_actions(exp)),
                   scm_sequence_exp(alt)));
}

static scm_object* while_to_more_prim(scm_object *exp)
{
    scm_object *var = scm_gen_symbol();
    scm_object *body = scm_while_body(exp);
    *(SCM_NULLP(body) ? &body : &SCM_CDR(body)) = SCM_LIST1(scm_make_app0(var));
    
    return scm_make_named_let(
               var,
               scm_null,
               scm_make_when(
                   scm_while_test(exp),
                   body));
}

static scm_object* for_to_more_prim(scm_object *exp)
{
    scm_object *var = scm_for_var(exp);
    scm_object *list = scm_for_list(exp);
    
    scm_object *loop_var = scm_gen_symbol();
    
    scm_object *body = scm_for_body(exp);
    *(SCM_NULLP(body) ? &body : &SCM_CDR(body)) =
        SCM_LIST1(
            scm_make_app(
                loop_var,
                SCM_LIST1(
                    scm_make_app(
                        scm_plus_symbol,
                        SCM_LIST2(var, scm_make_integer(1))))));

    return scm_make_named_let(
               loop_var,
               SCM_LIST1(SCM_LIST2(var, scm_for_list_start(list))),
               scm_make_when(
                   scm_make_app(scm_lt_symbol, SCM_LIST1(var, scm_for_list_end(list))),
                   body));
}