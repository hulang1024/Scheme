#include <stdio.h>
#include "scm.h"

#define cons(car, cdr) scm_make_pair(car, cdr)

scm_object scm_null[1];

scm_object* scm_make_pair(scm_object *car, scm_object *cdr)
{
    scm_object *pair = scm_malloc_object(sizeof(scm_pair));
    pair->type = scm_pair_type;
    SCM_CAR(pair) = car;
    SCM_CDR(pair) = cdr;
    return pair;
}

scm_object* scm_build_list(int size, scm_object **argv)
{
    scm_object *pair = scm_null;
    int i;

    for (i = size; i--; ) {
        pair = cons(argv[i], pair);
    }

    return pair;
}

int scm_list_length(scm_object *list)
{
    int len = 0;
    while(!SCM_NULLP(list)) {
        len++;
        if(SCM_PAIRP(list))
            list = SCM_CDR(list);
        else
            list = scm_null;
    }
    return len;
}