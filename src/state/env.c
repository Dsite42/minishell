/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:30:36 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/01 15:43:41 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"

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
		var->flags |= VAR_EXPORT;
		envp++;
	}
	return (1);
}

// TODO: Implement this
char	**envp_from_vars(t_var **p_root_var)
{
	(void) p_root_var;
	return (NULL);
}
