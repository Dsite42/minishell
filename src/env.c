/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:30:36 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 13:22:50 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "slice.h"

static int	is_delimiter(char input)
{
	return (input == '=');
}

int	vars_from_envp(char *envp[], t_var **p_root_var)
{
	t_slice	name;
	t_slice	value;
	t_var	*var;

	*p_root_var = NULL;
	while (*envp)
	{
		split_once(slice0(*envp), is_delimiter, &name, &value);
		value = advance(value);
		var = vars_set(p_root_var, name, value);
		if (var == NULL)
		{
			vars_clr(p_root_var);
			return (0);
		}
		var->flags |= VAR_EXPORT;
		envp++;
	}
	return (1);
}

char	**envp_from_vars(t_var **p_root_var)
{
	(void) p_root_var;
	// TODO: Implement this
	return (NULL);
}
