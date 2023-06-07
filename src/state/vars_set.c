/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:09:56 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/20 12:17:35 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
#include <stdlib.h>

static void	insert_var_alphabetically(t_var **p_root_var, t_var *var)
{
	t_var	*current;

	if (*p_root_var == NULL || ms_strncmp(var->name, (*p_root_var)->name,
			ms_strlen(var->name) + 1) < 0)
	{
		var->next = *p_root_var;
		*p_root_var = var;
	}
	else
	{
		current = *p_root_var;
		while (current->next != NULL && ms_strncmp(var->name,
				current->next->name, ms_strlen(var->name) + 1) > 0)
		{
			current = current->next;
		}
		var->next = current->next;
		current->next = var;
	}
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
	insert_var_alphabetically(p_root_var, var);
	return (var);
}
