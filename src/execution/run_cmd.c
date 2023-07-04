/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:56:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 11:56:56 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../state/state.h"
#include "../parser/parser.h"
#include "../builtin/builtin.h"
#include "execution.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

void	pipex_error(int shall_exit, char *message,
			int isstrerror, int exit_code)
{
	if (isstrerror == 1)
		print_fd(2, "minishell: %s %s\n", message, strerror(exit_code));
	else
		print_fd(2, "minishell: %s\n", message);
	if (shall_exit == 1)
		exit(exit_code);
}

void	error_cmd_not_found(char *cmd)
{
	print_fd(2, "minishell: %s: command not found\n", cmd);
	exit(127);
}

static int	count_cmds(t_cmd *cmd_root)
{
	int	counter;

	counter = 0;
	while (cmd_root != NULL)
	{
		counter++;
		cmd_root = cmd_root->next;
	}
	return (counter);
}

void	create_pipe(int i, int num_cmds, int (*pipefd)[2])
{
	if (i < num_cmds - 1)
	{
		if (pipe(*pipefd) == -1)
			pipex_error(1, "create pipe error", 1, errno);
	}
	else
	{
		(*pipefd)[0] = -1;
		(*pipefd)[1] = -1;
	}
}

static void	init_piping_data(t_piping *piping_data, t_cmd *first_cmd)
{
	piping_data->cmd = first_cmd;
	piping_data->num_cmds = count_cmds(first_cmd);
	piping_data->prev_read = STDIN_FILENO;
	piping_data->i = 0;
}

void	run_cmds(t_cmd *root_cmd, char **envp, t_state *state)
{
	t_piping	piping_data;
	pid_t		pid;
	int			status;

	init_piping_data(&piping_data, root_cmd);
	check_heredoc(piping_data);
	while (piping_data.i < piping_data.num_cmds)
	{
		create_pipe(piping_data.i, piping_data.num_cmds, &piping_data.pipefd);
		pid = fork();
		if (pid == -1)
			pipex_error(1, "fork error", 1, errno);
		else if (pid == 0)
			child(piping_data.cmd->argv, envp, &piping_data, state);
		else
			parent(&piping_data, state);
		piping_data.i++;
		if (piping_data.cmd->next != NULL)
			piping_data.cmd = piping_data.cmd->next;
	}
	while (piping_data.num_cmds > 0)
	{
		wait(&status);
		if (WIFEXITED(status))
			vars_set(&(state->root_var), slice0("?"),
				slice0(ms_int_to_str(WEXITSTATUS(status))));
		piping_data.num_cmds--;
	}
}
