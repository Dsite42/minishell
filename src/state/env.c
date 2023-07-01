/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:30:36 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/23 00:05:54 by jsprenge         ###   ########.fr       */
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

static int	populate_envp(void *context_ptr, t_var *var)
{
	char	***p_iter;
	size_t	new_length;
	size_t	name_length;
	size_t	value_length;
	char	*new_string;

	p_iter = context_ptr;
	name_length = ms_str_length(var->name);
	value_length = ms_str_length(var->value);
	new_length = name_length + 1 + value_length;
	new_string = malloc(new_length + 1);
	if (new_string == NULL)
		return (0);
	ms_copy(&new_string[0], var->name, name_length);
	new_string[name_length] = '=';
	ms_copy(&new_string[name_length + 1], var->value, value_length);
	new_string[new_length] = '\0';
	*((*p_iter)++) = new_string;
	return (1);
}

char	**envp_from_vars(t_var **p_root_var)
{
	size_t	count;
	char	**iter;
	char	**new_envp;

	iter_export_vars(&count, p_root_var, NULL, &count);
	new_envp = ms_zero_alloc(sizeof(char *), count);
	if (new_envp == NULL)
		return (NULL);
	iter = new_envp;
	if (!iter_export_vars(&iter, p_root_var, populate_envp, NULL))
	{
		ms_ptrs_free(new_envp);
		return (NULL);
	}
	return (new_envp);
}

int	iter_export_vars(void *context_ptr, t_var **p_root_var,
			int (*callback)(void *context_ptr, t_var *var),
			size_t *p_count)
{
	size_t	count;
	int		result;
	t_var	*head_var;

	count = 0;
	head_var = *p_root_var;
	while (head_var != NULL)
	{
		if (head_var->flags & VAR_EXPORT && (*head_var->value != '\0'
				|| head_var->flags & VAR_EXPLICIT_EMPTY))
		{
			if (callback != NULL)
			{
				result = callback(context_ptr, head_var);
				if (!result)
					return (result);
			}
			count++;
		}
		head_var = head_var->next;
	}
	if (p_count != NULL)
		*p_count = count;
	return (1);
}
