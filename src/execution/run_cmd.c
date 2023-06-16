/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:56:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/16 11:03:35 by cgodecke         ###   ########.fr       */
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
#include <stdio.h>

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

int	count_cmds(t_cmd *cmd_root)
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
	if (i < num_cmds - 1) // Create a pipe for commands except the last one
	{
		if (pipe(*pipefd) == -1)
			pipex_error(1, "create pipe error", 1, errno);
	}
}

void	child(char **envp, t_cmd cmd, t_piping piping_data, t_state *state)
{
	int		fd_dup[2];
	char	*path_cmd;

    // Set up input redirection
	if (piping_data.i > 0)
	{
		fd_dup[0] = dup2(piping_data.prev_read, STDIN_FILENO);
		if (fd_dup[0] == -1)
			pipex_error(1, "dup21 error", 1, errno);
		close(piping_data.prev_read);
	}
	// Set up output redirection
	if (piping_data.i < piping_data.num_cmds - 1)
	{
		fd_dup[1] = dup2(piping_data.pipefd[1], STDOUT_FILENO);
		if (fd_dup[1] == -1)
			pipex_error(1, "dup21 error", 1, errno);
		close(piping_data.pipefd[0]);
		close(piping_data.pipefd[1]);
	}
	path_cmd = get_path_cmd(cmd.argv, state);
	print_fd(1, "path_cmd:%s\n", path_cmd);
	if (execve((const char *) path_cmd, 
			cmd.argv, envp) == -1)
		pipex_error(0, "execve child error.", 1, errno);
}

void	parent(t_piping *piping_data)
{
    if (piping_data->i > 0)
        close(piping_data->prev_read);
    if (piping_data->i < piping_data->num_cmds - 1)
	{
        close(piping_data->pipefd[1]);
        piping_data->prev_read = piping_data->pipefd[0];
    }
}

void	run_cmds(char **argv, char **envp, t_state *state)
{
	t_cmd second = {
			.next = NULL,
			.root_redir = NULL,
			.argv = {"wc", "-c", NULL}
		};
	t_cmd	first = {
		.next = &second,
		.root_redir = NULL,
		.argv = {"ls", NULL}
	};


	print_fd(1, "t_cmd:%i\n", count_cmds(&first));

	t_piping	piping_data;
	pid_t	pid;

	piping_data.num_cmds = count_cmds(&first);  // Number of commands specified in argv	
	piping_data.prev_read = STDIN_FILENO;
	piping_data.i = 0;

    while (piping_data.i < piping_data.num_cmds)
	{
		create_pipe(piping_data.i, piping_data.num_cmds, &piping_data.pipefd);
		pid = fork();
		if (pid == -1) 
			pipex_error(1, "fork error", 1, errno);
		else if (pid == 0)
		{
			child(envp, first, piping_data, state);
        }
		else
			parent(&piping_data);
        piping_data.i++;
		if (first.next != NULL)
			first = *(first.next);
    }

    // Wait for all child processes to finish
    while (piping_data.num_cmds > 0)
	{
        int status;
        wait(&status);
        piping_data.num_cmds--;
    }
}
