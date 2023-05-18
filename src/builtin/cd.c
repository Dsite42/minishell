/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:47:53 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/18 16:47:45 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>

int	builtin_cd(int argc, char *argv[], int out_fd, t_state *state)
{
	char	*errormessage;
	char	buffer[MAXPATHLEN + 1];
	t_var	*var;

	(void) buffer;
	(void) argc;
	if (chdir(argv[2]) != 0)
	{
		errormessage = strerror(errno);
		print_fd(out_fd, "minishell: cd: %s: %s\n", argv[2], errormessage);
		return (1);
	}
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
