#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#include "vars.h"
#include "util.h"

int main(void) {
    t_var *root_var = NULL, *new_var;

    new_var = vars_set(&root_var, slice0("PATH"), slice0("/bin:/sbin:/usr/bin"));
    assert(new_var != NULL);

    new_var = vars_set(&root_var, slice0("NAME"), slice0("overwrite me"));
    assert(new_var != NULL);

    new_var = vars_set(&root_var, slice0("DISPLAY"), slice0(":0"));
    assert(new_var != NULL);

    new_var = vars_set(&root_var, slice0("NAME"), slice0("jsprenge"));
    assert(new_var != NULL);

    new_var = vars_get(&root_var, slice0("NAME"));
    printf("NAME = %s\n", new_var ? new_var->value : "(nil)");

    vars_clr(&root_var);

    return 0;
}