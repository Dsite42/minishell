/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:02:56 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/04 21:29:29 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../parser/parser.h"

#include <unistd.h>
#include <stdlib.h>

static void	handle_exit_code_argument(const char *argument, int *p_exit_code)
{
	t_slice	slice;

	slice = trim_left(slice0(argument), begin_space, NULL);
	if (!parse_int(p_exit_code, &slice))
	{
		print_fd(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", argument);
		*p_exit_code = 255;
	}
}

int	builtin_exit(int argc, char *argv[], int out_fd, t_state *state)
{
	int	exit_code;

	(void) out_fd;
	print_fd(STDERR_FILENO, "exit\n");
	if (argc == 1)
		exit_code = state->exit_code;
	else if (argc == 2)
		handle_exit_code_argument(argv[1], &exit_code);
	else
	{
		exit_code = 1;
		print_fd(STDERR_FILENO, "minishell: exit: too many arguments\n");
	}
	state_drop(state);
	exit(exit_code);
	return (0);
}
