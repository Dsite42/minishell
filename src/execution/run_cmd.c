/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:56:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/29 16:03:53 by cgodecke         ###   ########.fr       */
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

void	pipex_error(int shall_exit, char *message,
			int isstrerror, int exit_code)
{
	if (isstrerror == 1)
		print_fd(2, "pipex: %s %s\n", message, strerror(exit_code));
	else
		print_fd(2, "pipex: %s\n", message);
	if (shall_exit == 1)
		exit(exit_code);
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

void	run_cmds(char **argv, char **envp, t_state *state)
{
	t_redir	fivth_redir = {
		.next = NULL,
		.type = WORD_OP_WRITE,
		.name = "output.txt"
	};

	t_redir	sixth_redir = {
		.next = NULL,
		.type = WORD_OP_READ,
		.name = "input.txt"
	};
	t_redir	fourth_redir = {
		.next = NULL,
		.type = WORD_OP_APPEND,
		.name = "output.txt"
	};
	t_redir	third_redir = {
		.next = NULL,
		.type = WORD_OP_WRITE,
		.name = "output3.txt"
	};
	t_redir	second_redir = {
		.next = NULL,
		.type = WORD_OP_WRITE,
		.name = "output2.txt"
	};
	t_redir	first_redir = {
		.next = NULL,
		.type = WORD_OP_HEREDOC,
		.name = "eof"
	};

	t_cmd third = {
		.next = NULL,
		.root_redir = NULL,
		.argv = {"pwd", NULL}
	};

	t_cmd second = {
			.next = &third,
			.root_redir = NULL,
			.argv = {"cd", "src", NULL}
		};
	t_cmd	first = {
		.next = &second,
		.root_redir = NULL,
		.argv = {"pwd", NULL}
	};


	t_piping	piping_data;
	pid_t		pid;
	int			status;

	init_piping_data(&piping_data, &first);
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
		piping_data.num_cmds--;
	}
}
