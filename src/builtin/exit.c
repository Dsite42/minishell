/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:02:56 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 20:22:51 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../parser/parser.h"

#include <unistd.h>
#include <stdlib.h>

static void	handle_exit_code_argument(const char *argument, int *p_exit_code)
{
	int		i;
	t_slice	slice;

	i = 0;
	if (argument[i] == '+' || argument[i] == '-')
		i++;
	while (argument[i] != '\0')
	{
		if (ms_is_digit((int)argument[i]) == 0)
		{
			print_fd(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", argument);
			*p_exit_code = 255;
			return ;
		}
		i++;
	}
	slice = trim_left(slice0(argument), begin_space, NULL);
	parse_int(p_exit_code, &slice);
}

int	builtin_exit(int argc, char *argv[], int out_fd, t_state *state)
{
	int	exit_code;

	(void) out_fd;
	print_fd(STDERR_FILENO, "exit\n");
	if (argc == 1)
		exit_code = 0;
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
