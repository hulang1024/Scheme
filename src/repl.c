#include <stdio.h>
#include "scm.h"
#include <assert.h>

int main(int argc, char *argv[])
{
    test();

    scm_env *global_env;
    scm_println( scm_eval( scm_make_integer(1), global_env) );
    
    return 0;
}