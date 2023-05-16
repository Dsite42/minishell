/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:47:53 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/16 20:05:00 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "vars.h"
#include "util.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>

int	builtin_cd(int argc, char *argv[], int out_fd, void *context)
{
	char	*errormessage;
	char	buffer[MAXPATHLEN + 1];
	t_var	*var;

	(void) buffer;
	(void) argc;
	(void) out_fd;
	(void) context;
		
	if (chdir(argv[2]) != 0)
	{
		errormessage = strerror(errno);
		print_fd(STDERR_FILENO, "minishell: cd: %s: %s\n", argv[2], errormessage);
		return (1);
	}
	if (getcwd(buffer, sizeof(buffer)) == NULL) // TODO: Error check
	{
		perror("getcwd() error");
		return (1);
	}
	/* TODO: Implement when context is implemented
	var = vars_set(&context->root_var, slice0("PWD"), slice0(buffer));
	if (var == NULL)
		return (1);
	if (var->flags & VAR_EXPORT)
	{
		builtin_export(argc, argv, out_fd, context); // uncomment when builtin_export is implemented
	}
	*/
	return (0);
}

