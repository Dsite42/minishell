/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:18:55 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/20 12:12:28 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
#include <stdlib.h>

t_var	*var_new(t_slice name, t_slice value)
{
	t_var	*new_var;

	new_var = malloc(sizeof(t_var) + name.size + 1);
	if (new_var == NULL)
		return (NULL);
	new_var->flags = 0;
	new_var->value = slice_to_str(value);
	if (new_var->value == NULL)
	{
		free(new_var);
		return (NULL);
	}
	ms_memcpy(new_var->name, name.data, name.size);
	new_var->name[name.size] = '\0';
	return (new_var);
}

t_var	*vars_get(t_var **p_root_var, t_slice name)
{
	t_var	*head_var;

	head_var = *p_root_var;
	while (head_var != NULL)
	{
		if (slice_str_equal(name, head_var->name))
			return (head_var);
		head_var = head_var->next;
	}
	return (NULL);
}

void	vars_del(t_var **p_root_var, t_slice name)
{
	t_var	*head_var;
	t_var	*last_var;

	head_var = *p_root_var;
	last_var = NULL;
	while (head_var != NULL)
	{
		if (slice_str_equal(name, head_var->name))
			break ;
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
