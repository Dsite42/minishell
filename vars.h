#ifndef VAR_H
#define VAR_H

#include "util.h"

typedef struct s_var {
    struct s_var *next;
    char         *value;
    size_t        name_len;
    char          name[];
} t_var;

t_var *vars_get(t_var **p_root_var, t_slice name);
t_var *vars_set(t_var **p_root_var, t_slice name, t_slice value);
void   vars_del(t_var **p_root_var, t_slice name);

#endif // !VAR_H