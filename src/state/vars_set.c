/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:09:56 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/04 20:51:43 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"

#include <stdlib.h>

static void	insert_var_alphabetically(t_var **p_root_var, t_var *var)
{
	t_var	*current;

	if (*p_root_var == NULL || ms_str_compare(
			var->name, (*p_root_var)->name, 0) < 0)
	{
		var->next = *p_root_var;
		*p_root_var = var;
	}
	else
	{
		current = *p_root_var;
		while (current->next != NULL && ms_str_compare(
				var->name, current->next->name, 0) > 0)
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
