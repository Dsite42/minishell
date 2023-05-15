/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/15 16:15:41 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#undef NDEBUG

#include <assert.h>
#include <stdio.h>

#include "vars.h"
#include "util.h"

static void	vars_dump(t_var **p_root_var, t_slice name)
{
	t_var	*var;

	var = vars_get(p_root_var, name);
	printf("* ");
	fwrite(name.data, name.size, 1, stdout);
	if (var == NULL)
		printf(" is unset!\n");
	else
		printf(" is set to '%s'\n", var->value);
}

int	main(void)
{
	t_var	*root_var;
	t_var	*new_var;

	root_var = NULL;
	new_var = vars_set(&root_var, slice0("PATH"), slice0("/bin:/usr/bin"));
	assert(new_var != NULL);
	new_var = vars_set(&root_var, slice0("NAME"), slice0("overwrite me"));
	assert(new_var != NULL);
	new_var = vars_set(&root_var, slice0("DISPLAY"), slice0(":0"));
	assert(new_var != NULL);
	new_var = vars_set(&root_var, slice0("NAME"), slice0("jsprenge"));
	assert(new_var != NULL);
	vars_dump(&root_var, slice0("PATH"));
	vars_dump(&root_var, slice0("NAME"));
	vars_dump(&root_var, slice0("DISPLAY"));
	vars_clr(&root_var);
	return (0);
}
