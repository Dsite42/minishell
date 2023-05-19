/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 13:49:38 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/18 16:50:57 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_env(int argc, char *argv[], int out_fd, t_state *state)
{
	t_var	*head_var;

	(void) argc;
	(void) argv;
	head_var = state->root_var;
	while (head_var != NULL)
	{
		if (head_var->flags & VAR_EXPORT)
			print_fd(out_fd, "%s=%s\n", head_var->name, head_var->value);
		head_var = head_var->next;
	}
	return (0);
}
