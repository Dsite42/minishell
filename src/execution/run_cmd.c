/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:56:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/15 16:18:20 by cgodecke         ###   ########.fr       */
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

void	run_cmds(char **argv, char **envp, t_state *state)
{
	t_cmd second = {
			.next = NULL,
			.root_redir = NULL,
			.argv = {"grep", "M", NULL}
		};
	t_cmd	first = {
		.next = &second,
		.root_redir = NULL,
		.argv = {"ls", NULL}
	};


	print_fd(1, "t_cmd:%i\n", count_cmds(&first));



	int		num_cmds = count_cmds(&first);  // Number of commands specified in argv	
	int		prev_read = STDIN_FILENO;  // Input source for the first command
	int		pipefd[2];  // Pipe file descriptors
	char	*path_cmd;
    int		i = 0;
    while (i < num_cmds)
	{
		if (i < num_cmds - 1)
		{
		    // Create a pipe for commands except the last one
		    if (pipe(pipefd) == -1)
			{
		        perror("pipe");
		        exit(EXIT_FAILURE);
		    }
		}

			// Fork a child process
			pid_t pid = fork();
			if (pid == -1) 
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
		else if (pid == 0)
		{
    		    // Child process
			int		fd_dup[2];

            // Set up input redirection
            if (i > 0)
			{
                // Redirect stdin to the read end of the previous pipe
                fd_dup[0] = dup2(prev_read, STDIN_FILENO);
				if (fd_dup[0] == -1)
					pipex_error(1, "dup21 error", 1, errno);
                close(prev_read);
            }

            // Set up output redirection
            if (i < num_cmds - 1)
			{
                // Redirect stdout to the write end of the current pipe
                fd_dup[1] = dup2(pipefd[1], STDOUT_FILENO);
				if (fd_dup[1] == -1)
					pipex_error(1, "dup21 error", 1, errno);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Execute the command
			//exit(0);
			path_cmd = get_path_cmd(first.argv, state);
			print_fd(1, "path_cmd:%s\n", path_cmd);
			if (execve((const char *) path_cmd, 
					first.argv, envp) == -1)
			{
				pipex_error(0, "execve child error.", 1, errno);
			}

			// execvp(*argv_child, argv_child);
            // If execvp returns, an error occurred
            perror("execvp");
            exit(EXIT_FAILURE);
        }
		else
		{
            // Parent process

            // Close unnecessary pipe ends
            if (i > 0)
			{
                close(prev_read);
            }
            if (i < num_cmds - 1)
			{
                close(pipefd[1]);
                prev_read = pipefd[0];
            }
        }
        i++;
		if (first.next != NULL)
			first = *(first.next);
    }

    // Wait for all child processes to finish
    while (num_cmds > 0)
	{
        int status;
        wait(&status);
        num_cmds--;
    }
}
