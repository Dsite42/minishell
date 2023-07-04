/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:56:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 19:32:34 by jsprenge         ###   ########.fr       */
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
			execution_error(1, "create pipe error", 1, errno);
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

void	waiting(t_piping piping_data, t_state *state)
{
	int		status;
	char	*exit_code;

	while (piping_data.num_cmds > 0)
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			exit_code = ms_int_to_str(WEXITSTATUS(status));
			vars_set(&(state->root_var), slice0("?"), slice0(exit_code));
			free(exit_code);
		}
		piping_data.num_cmds--;
	}
}

void	run_cmds(t_cmd *root_cmd, char **envp, t_state *state)
{
	t_piping	piping_data;
	pid_t		pid;

	init_piping_data(&piping_data, root_cmd);
	if (!check_heredoc(piping_data))
		return ((void) dup2(state->saved_stdin, STDIN_FILENO));
	tty_enter(1);
	while (piping_data.i < piping_data.num_cmds)
	{
		create_pipe(piping_data.i, piping_data.num_cmds, &piping_data.pipefd);
		pid = fork();
		if (pid == -1)
			execution_error(1, "fork error", 1, errno);
		else if (pid == 0)
			child(piping_data.cmd->argv, envp, &piping_data, state);
		else
			parent(&piping_data, state);
		piping_data.i++;
		if (piping_data.cmd->next != NULL)
			piping_data.cmd = piping_data.cmd->next;
	}
	waiting(piping_data, state);
	tty_enter(0);
}
