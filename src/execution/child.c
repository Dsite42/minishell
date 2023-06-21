/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:59:08 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/21 17:17:49 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "../builtin/builtin.h"
#include <stdlib.h>

static int	is_write_or_append(t_redir *root_redir)
{
	while (root_redir != NULL)
	{
		if (root_redir->type == WORD_OP_WRITE
			|| root_redir->type == WORD_OP_APPEND)
			return (1);
		root_redir = root_redir->next;
	}
	return (0);
}

static void	dup_write_append(t_piping *piping_data, int *fd_dup)
{
	int	fd_outfile;

	while (piping_data->cmd->root_redir != NULL)
	{
		if (piping_data->cmd->root_redir->type == WORD_OP_WRITE
			|| piping_data->cmd->root_redir->type == WORD_OP_APPEND)
		{
			if (piping_data->cmd->root_redir->type == WORD_OP_WRITE)
				fd_outfile = open(piping_data->cmd->root_redir->name,
						O_CREAT | O_RDWR | O_TRUNC, 0644);
			else if (piping_data->cmd->root_redir->type == WORD_OP_APPEND)
				fd_outfile = open(piping_data->cmd->root_redir->name,
						O_CREAT | O_APPEND | O_WRONLY, 0644);
			if (fd_outfile == -1)
				pipex_error(1, "fd_outfile error:", 1, errno);
			fd_dup[1] = dup2(fd_outfile, STDOUT_FILENO);
			if (fd_dup[1] == -1)
				pipex_error(1, "dup_output_1 error", 1, errno);
			close(piping_data->pipefd[0]);
			close(fd_outfile);
		}
		piping_data->cmd->root_redir = piping_data->cmd->root_redir->next;
	}
}

static void	output_redirection(t_piping *piping_data, int *fd_dup)
{
	if (is_write_or_append(piping_data->cmd->root_redir))
	{
		dup_write_append(piping_data, fd_dup);
	}
	else if (piping_data->i < piping_data->num_cmds - 1)
	{
		fd_dup[1] = dup2(piping_data->pipefd[1], STDOUT_FILENO);
		if (fd_dup[1] == -1)
			pipex_error(1, "dup_output_2 error", 1, errno);
		close(piping_data->pipefd[0]);
		close(piping_data->pipefd[1]);
	}
}

int	run_builtin(char **argv, t_state *state)
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

void	child(char **argv, char **envp, t_piping *piping_data, t_state *state)
{
	int		fd_dup[2];
	char	*path_cmd;


	input_redirection(piping_data, fd_dup);
	output_redirection(piping_data, fd_dup);
	if (run_builtin(argv, state) == 127)
	{
		path_cmd = get_path_cmd(piping_data->cmd->argv, state);
		if (execve((const char *) path_cmd,
				piping_data->cmd->argv, envp) == -1)
			pipex_error(0, "execve child error.", 1, errno);
	}
	else
		exit(10);
}
