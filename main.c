#undef NDEBUG
#include <assert.h>

#include "vars.h"
#include "util.h"

int main(void) {
    t_var *root_var = NULL, *new_var;

    new_var = vars_set(&root_var, slice0("TEST"), slice0("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
    assert(new_var != NULL);

    return 0;
}