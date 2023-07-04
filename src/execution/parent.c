/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:22:16 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/05 01:10:45 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>
#include "../builtin/builtin.h"
#include <errno.h>
#include <stdlib.h>

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

int	parent(t_piping *piping_data, t_state *state)
{
	int		fd_dup[2];
	char	*return_value;

	if (*piping_data->cmd->argv != NULL
		&& is_builtin(piping_data->cmd->argv) == 1
		&& piping_data->num_cmds == 1)
	{
		input_redirection(piping_data, fd_dup);
		output_redirection(piping_data, fd_dup);
		return_value = ms_int_to_str(
				run_builtin(piping_data->cmd->argv, state));
		vars_set(&state->root_var, slice0("?"), slice0(return_value));
		free(return_value);
		if (dup2(state->saved_stdout, STDOUT_FILENO) == -1)
			execution_error(1, "dup2_parent_output_error", 1, errno);
		return (1);
	}
	if (piping_data->i > 0)
		close(piping_data->prev_read);
	if (piping_data->i < piping_data->num_cmds - 1)
	{
		close(piping_data->pipefd[1]);
		piping_data->prev_read = piping_data->pipefd[0];
	}
	return (0);
}
