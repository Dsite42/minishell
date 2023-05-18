/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 13:49:38 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/18 16:37:51 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_env(int argc, char *argv[], int out_fd, t_state *state)
{
	t_var	*root_var;

	(void) argc;
	(void) argv;
	root_var = state->root_var;
	while (root_var != NULL)
	{
		if (root_var->flags & VAR_EXPORT)
			print_fd(out_fd, "%s=%s\n", root_var->name, root_var->value);
		root_var = root_var->next;
	}
	return (0);
}
