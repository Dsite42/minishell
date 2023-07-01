/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:47:53 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/01 19:47:52 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>

static int	update_cwd_var(t_state *state)
{
	t_var	*var;
	char	buffer[MAXPATHLEN + 1];

	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("getcwd() error");
		return (1);
	}
	var = vars_set(&state->root_var, slice0("PWD"), slice0(buffer));
	if (var == NULL)
		return (1);
	return (0);
}

int	builtin_cd(int argc, char *argv[], int out_fd, t_state *state)
{
	(void) out_fd;
	if (argc < 2)
	{
		print_fd(STDERR_FILENO, "minishell: cd: too few arguments\n");
		return (1);
	}
	if (argc > 2)
	{
		print_fd(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (chdir(argv[1]) != 0)
	{
		print_fd(STDERR_FILENO,
			"minishell: cd: %s: %s\n", argv[1], strerror(errno));
		return (1);
	}
	return (update_cwd_var(state));
}
