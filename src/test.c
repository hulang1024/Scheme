#include <stdio.h>
#include <assert.h>
#include "scm.h"

void test()
{
    /// tests
    
    scm_object *n1 = scm_make_integer(1);
    scm_object *n2 = scm_make_integer(2);
    
    scm_println(n1);
    scm_println(scm_true);
    scm_println(scm_false);
    scm_println(scm_null);
    
    scm_object *bool_pair = cons(scm_true, scm_false);
    scm_object *list1 = cons(n1, cons(n2, cons(bool_pair, scm_null)));
    
    scm_println(list1);
    
    assert(scm_list_length(bool_pair) == 2);
    assert(scm_list_length(list1) == 3);
    
    scm_object *n3 = scm_make_integer(3);
    scm_object *n4 = scm_make_integer(4);
    
    SCM_CAR(list1) = n3;
    SCM_CDR(list1) = n4;
    
    assert(scm_equal(SCM_CAR(list1), n3));
    assert(scm_equal(SCM_CDR(list1), n4));
    
    scm_object *objs[] = {scm_true, scm_false};
    scm_println(scm_build_list(2, objs));

}