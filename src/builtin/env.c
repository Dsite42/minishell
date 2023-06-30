/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 13:49:38 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/23 00:05:35 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	print_var(void *context_ptr, t_var *var)
{
	int	out_fd;

	out_fd = *(int *) context_ptr;
	print_fd(out_fd, "%s=%s\n", var->name, var->value);
	return (1);
}

int	builtin_env(int argc, char *argv[], int out_fd, t_state *state)
{
	(void) argc;
	(void) argv;
	(void) state;
	iter_export_vars(&out_fd, &state->root_var, print_var, NULL);
	return (0);
}
