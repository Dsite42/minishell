/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:18:55 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/15 16:20:18 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vars.h"

#include <string.h>
#include <stdlib.h>

static t_var	*var_new(t_slice name, t_slice value)
{
	t_var	*new_var;

	new_var = malloc(sizeof(t_var) + name.size);
	if (new_var == NULL)
		return (NULL);
	new_var->value = slice_to_str(value);
	if (new_var->value == NULL)
	{
		free(new_var);
		return (NULL);
	}
	new_var->name_len = name.size;
	memcpy(new_var->name, name.data, name.size);
	return (new_var);
}

t_var	*vars_get(t_var **p_root_var, t_slice name)
{
	t_var	*head_var;

	head_var = *p_root_var;
	while (head_var != NULL)
	{
		if (head_var->name_len == name.size)
		{
			if (memcmp(head_var->name, name.data, name.size) == 0)
				return (head_var);
		}
		head_var = head_var->next;
	}
	return (NULL);
}

t_var	*vars_set(t_var **p_root_var, t_slice name, t_slice value)
{
	t_var	*var;

	var = vars_get(p_root_var, name);
	if (var != NULL)
	{
		free(var->value);
		var->value = slice_to_str(value);
		return (var);
	}
	var = var_new(name, value);
	if (var == NULL)
		return (NULL);
	var->next = *p_root_var;
	*p_root_var = var;
	return (var);
}

void	vars_del(t_var **p_root_var, t_slice name)
{
	t_var	*head_var;
	t_var	*last_var;

	head_var = *p_root_var;
	last_var = NULL;
	while (head_var != NULL)
	{
		if (head_var->name_len == name.size)
		{
			if (memcmp(head_var->name, name.data, name.size) == 0)
				break ;
		}
		last_var = head_var;
		head_var = head_var->next;
	}
	if (head_var == NULL)
		return ;
	if (last_var == NULL)
		*p_root_var = head_var->next;
	else
		last_var->next = head_var->next;
	free(head_var->value);
	free(head_var);
}

void	vars_clr(t_var **p_root_var)
{
	t_var	*head_var;
	t_var	*next_var;

	head_var = *p_root_var;
	while (head_var != NULL)
	{
		next_var = head_var->next;
		free(head_var->value);
		free(head_var);
		head_var = next_var;
	}
	*p_root_var = NULL;
}
