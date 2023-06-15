/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:30:36 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/15 13:28:59 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
#include <stdlib.h>

size_t	begin_delimiter(t_slice slice)
{
	return (slice.size > 0 && *slice.data == '=');
}

int	vars_from_envp(char *envp[], t_var **p_root_var)
{
	t_slice	name;
	t_slice	value;
	t_var	*var;

	*p_root_var = NULL;
	while (*envp)
	{
		split_once(slice0(*envp), begin_delimiter, &name, &value);
		value = advance(value);
		var = vars_set(p_root_var, name, value);
		if (var == NULL)
		{
			vars_clr(p_root_var);
			return (0);
		}
		var->flags |= VAR_EXPORT | VAR_EXPLICIT_EMPTY;
		envp++;
	}
	return (1);
}

// TODO: Implement this
static int	count_envps(t_var *p_root_var)
{
	int		envs_counter;

	envs_counter = 0;
	while (p_root_var != NULL)
	{
		if (p_root_var->flags & VAR_EXPORT
			&& p_root_var->flags & VAR_EXPLICIT_EMPTY)
			envs_counter++;
		p_root_var = p_root_var->next;
	}
	return (envs_counter);
}

char	**envp_from_vars(t_var **p_root_var)
{
	t_var	*head_var;
	char	**envp;
	char	**envp_start;
	int		envs_counter;

	head_var = *p_root_var;
	envs_counter = count_envps(head_var);
	envp = (char **)malloc((envs_counter + 1) * sizeof (char *));
	if (envp == NULL)
		return (NULL);
	envp_start = envp;
	head_var = *p_root_var;
	while (head_var != NULL)
	{
		if (head_var->flags & VAR_EXPORT)
		{
			*envp = ms_two_strjoin(head_var->name, head_var->value, "=");
			envp++;
		}
		head_var = head_var->next;
	}
	*envp = NULL;
	return (envp_start);
}
