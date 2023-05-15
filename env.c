/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:30:36 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/15 19:24:59 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "slice.h"

static int	is_delimiter(char input)
{
	return (input == '=');
}

t_var	*vars_from_envp(char *envp[])
{
	t_slice	name;
	t_slice	value;
	t_var	*root_var;
	t_var	*var;

	while (*envp)
	{
		split_once(slice0(*envp), is_delimiter, &name, &value);
		value = advance(value);
		var = vars_set(&root_var, name, value);
		if (var == NULL)
		{
			vars_clr(&root_var);
			return (NULL);
		}
		var->flags |= VAR_EXPORT;
		envp++;
	}
	return (root_var);
}

char	**envp_from_vars(t_var **p_root_var)
{
	(void) p_root_var;
	// TODO: Implement this
	return (NULL);
}
