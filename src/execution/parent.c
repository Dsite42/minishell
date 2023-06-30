/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:22:16 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/30 14:51:15 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>
#include "../builtin/builtin.h"


static int	run_builtin(char **argv, t_state *state)
{
	int	count;

	count = (int) ms_ptrs_count(argv);
	if (ms_str_compare(argv[0], "cd", 0) == 0)
		return (builtin_cd(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "echo", 0) == 0)
		return (builtin_echo(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "env", 0) == 0)
		return (builtin_env(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "exit", 0) == 0)
		return (builtin_exit(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "export", 0) == 0)
		return (builtin_export(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "pwd", 0) == 0)
		return (builtin_pwd(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "unset", 0) == 0)
		return (builtin_unset(count, argv, STDOUT_FILENO, state));
	return (127);
}

static int	is_builtin(char **argv)
{
	if (ms_str_compare(argv[0], "cd", 0) == 0)
		return (1);
	if (ms_str_compare(argv[0], "echo", 0) == 0)
		return (1);
	if (ms_str_compare(argv[0], "env", 0) == 0)
		return (1);
	if (ms_str_compare(argv[0], "exit", 0) == 0)
		return (1);
	if (ms_str_compare(argv[0], "export", 0) == 0)
		return (1);
	if (ms_str_compare(argv[0], "pwd", 0) == 0)
		return (1);
	if (ms_str_compare(argv[0], "unset", 0) == 0)
		return (1);
	return (0);
}

void	parent(t_piping *piping_data, t_state *state)
{
	int	fd_dup[2];

	if (is_builtin(piping_data->cmd->argv) == 1 && piping_data->num_cmds == 1)
	{
		input_redirection(piping_data, fd_dup);
		output_redirection(piping_data, fd_dup);
		run_builtin(piping_data->cmd->argv, state);
		dup2(state->saved_STDOUT_FILENO, STDOUT_FILENO);
	}

	if (piping_data->i > 0)
		close(piping_data->prev_read);
	if (piping_data->i < piping_data->num_cmds - 1)
	{
		close(piping_data->pipefd[1]);
		piping_data->prev_read = piping_data->pipefd[0];
	}
}
