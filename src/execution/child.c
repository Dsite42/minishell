/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:59:08 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/29 17:23:46 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "../builtin/builtin.h"
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

void	child(char **argv, char **envp, t_piping *piping_data, t_state *state)
{
	int		fd_dup[2];
	char	*path_cmd;

	input_redirection(piping_data, fd_dup);
	output_redirection(piping_data, fd_dup);
	if (is_builtin(piping_data->cmd->argv) == 1 && piping_data->num_cmds > 1)
	{
		if (run_builtin(argv, state) == 127)
			exit(10);
	}
	else if (is_builtin(piping_data->cmd->argv) == 0)
	{
		path_cmd = get_path_cmd(piping_data->cmd->argv, state);
		if (execve((const char *) path_cmd,
				piping_data->cmd->argv, envp) == -1)
			pipex_error(0, "execve child error.", 1, errno);
	}
}
