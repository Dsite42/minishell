/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 13:49:38 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/18 15:53:41 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "util.h"

int	builtin_env(int argc, char *argv[], int out_fd, void *context)
{
	t_var	*root_var;

	(void)argc;
	(void)argv;
	(void)context;

	root_var = ((t_storage *) context)->root_var;
	while (root_var != NULL)
	{
		if (root_var->flags & VAR_EXPORT)
			print_fd(out_fd, "%s=%s\n", root_var->name, root_var->value);
		root_var = root_var->next;
	}
	return (0);
}
