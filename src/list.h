#ifndef SCHEME_LIST_H
#define SCHEME_LIST_H

#include "scm.h"

#define cons(car, cdr) scm_make_pair(car, cdr)
#define SCM_CONS(car, cdr) cons(car, cdr)
#define SCM_LCONS(car, cdr) scm_make_list_pair(car, cdr)
#define SCM_LIST1(a) SCM_LCONS(a, scm_null)
#define SCM_LIST2(a, b) SCM_LCONS(a, SCM_LIST1(b))
#define SCM_LIST3(a, b, c) SCM_LCONS(a, SCM_LIST2(b, c))
#define SCM_LIST4(a, b, c, d) SCM_LCONS(a, SCM_LIST3(b, c, d))
#define SCM_CAAR(o) SCM_CAR(SCM_CAR(o))
#define SCM_CADR(o) SCM_CAR(SCM_CDR(o))
#define SCM_CDDR(o) SCM_CDR(SCM_CDR(o))
#define SCM_CDAR(o) SCM_CDR(SCM_CAR(o))
#define SCM_CAADR(o) SCM_CAR(SCM_CADR(o))
#define SCM_CADAR(o) SCM_CAR(SCM_CDAR(o))
#define SCM_CADDR(o) SCM_CAR(SCM_CDDR(o))
#define SCM_CDDDR(o) SCM_CDR(SCM_CDDR(o))
#define SCM_CDADR(o) SCM_CDR(SCM_CADR(o))
#define SCM_CDDAR(o) SCM_CDR(SCM_CDAR(o))
#define SCM_CADDDR(o) SCM_CAR(SCM_CDDDR(o))
#define SCM_CDDDDR(o) SCM_CDR(SCM_CDDDR(o))
#define SCM_CAADDR(o) SCM_CAR(SCM_CADDR(o))
#define SCM_CDADDR(o) SCM_CDR(SCM_CADDR(o))
#define SCM_CADDAR(o) SCM_CAR(SCM_CDDAR(o))

/*
 * iterate over a list
 * @list  the proper list variable
 */
#define scm_list_for_each(list) \
    assert(scm_is_list(list)); \
    for (; !SCM_NULLP(list); list = SCM_CDR(list))

extern scm_object scm_null[];

void scm_init_list(scm_env *env);
scm_object* scm_make_pair(scm_object *, scm_object *);
scm_object* scm_make_list_pair(scm_object *, scm_object *);
scm_object* scm_build_list(int, scm_object **);
scm_object* scm_append_list2(scm_object *, scm_object *);
int scm_list_length(scm_object *);
int scm_is_list(scm_object *);

#endif //SCHEME_LIST_H
