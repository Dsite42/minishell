/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:56:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/19 19:38:04 by cgodecke         ###   ########.fr       */
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
#include <fcntl.h>

#include <readline/history.h>
#include <readline/readline.h>


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

void	child(char **envp, t_piping *piping_data, t_state *state)
{
	int		fd_dup[2];
	int		fd_infile;
	int		fd_outfile;
	char	*path_cmd;

    // Set up input redirection
	if (piping_data->cmd->root_redir != NULL && piping_data->cmd->root_redir->type == WORD_OP_READ)
	{
		fd_infile = open(piping_data->cmd->root_redir->name, O_RDONLY);
		if (fd_infile == -1)
			pipex_error(1, "input:", 1, errno);
		fd_dup[0] = dup2(fd_infile, STDIN_FILENO);
		close(fd_infile);
		piping_data->cmd->root_redir = piping_data->cmd->root_redir->next;
	}
	else if (piping_data->cmd->root_redir != NULL && piping_data->cmd->root_redir->type == WORD_OP_HEREDOC)
	{
		fd_infile = piping_data->cmd->root_redir->pipefd_heredoc[0];
		if (fd_infile == -1)
			pipex_error(1, "input:", 1, errno);
		fd_dup[0] = dup2(fd_infile, STDIN_FILENO);
		close(fd_infile);
		piping_data->cmd->root_redir = piping_data->cmd->root_redir->next;
	}
	else
	{
		fd_dup[0] = dup2(piping_data->prev_read, STDIN_FILENO);
		close(piping_data->prev_read);
	}
	if (fd_dup[0] == -1)
		pipex_error(1, "dup21 error", 1, errno);

	// Set up output redirection
		
	if (piping_data->cmd->root_redir != NULL)
	{
		if (piping_data->cmd->root_redir->type == WORD_OP_WRITE)
			fd_outfile = open(piping_data->cmd->root_redir->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (piping_data->cmd->root_redir->type == WORD_OP_APPEND)
			fd_outfile = open(piping_data->cmd->root_redir->name, O_CREAT | O_APPEND | O_WRONLY, 0644);
		fd_dup[1] = dup2(fd_outfile, STDOUT_FILENO);
		if (fd_dup[1] == -1)
			pipex_error(1, "dup21 error", 1, errno);
		close(piping_data->pipefd[0]);
		close(fd_outfile);
	}
	else if (piping_data->i < piping_data->num_cmds - 1)
	{
		fd_dup[1] = dup2(piping_data->pipefd[1], STDOUT_FILENO);
		if (fd_dup[1] == -1)
			pipex_error(1, "dup21 error", 1, errno);
		close(piping_data->pipefd[0]);
		close(piping_data->pipefd[1]);
	}

	path_cmd = get_path_cmd(piping_data->cmd->argv, state);
	//print_fd(1, "path_cmd:%s\n", path_cmd);
	if (execve((const char *) path_cmd, 
			piping_data->cmd->argv, envp) == -1)
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

void	check_heredoc(t_piping piping_data)
{
	char *line;
	t_redir *root_dir_start;
	t_cmd	*cmd_start;

	cmd_start = piping_data.cmd;
	while (piping_data.cmd != NULL)
	{
		root_dir_start = piping_data.cmd->root_redir;
		while (piping_data.cmd->root_redir != NULL )
		{
			if (piping_data.cmd->root_redir->type == WORD_OP_HEREDOC)
			{
				create_pipe(0, 2, &piping_data.cmd->root_redir->pipefd_heredoc);
				while (1)
				{
					line = readline("heredoc> ");
					if (ms_str_compare(line, piping_data.cmd->root_redir->name, ms_str_length(piping_data.cmd->root_redir->name)) == 0)
					{
						free(line);
						close(piping_data.cmd->root_redir->pipefd_heredoc[1]);
						break ;
					}
					else
						print_fd(piping_data.cmd->root_redir->pipefd_heredoc[1], "%s\n", line);

					free(line);
				}
			}
			piping_data.cmd->root_redir = piping_data.cmd->root_redir->next;
		}
		piping_data.cmd->root_redir = root_dir_start;
		piping_data.cmd = piping_data.cmd->next;
	}
	piping_data.cmd = cmd_start;
}

void	run_cmds(char **argv, char **envp, t_state *state)
{
	t_redir	first_output = {
		.next = NULL,
		.type = WORD_OP_WRITE,
		.name = "output.txt"
	};
	t_redir	first_input = {
		.next = NULL,
		.type = WORD_OP_HEREDOC,
		.name = "eof"
	};
	t_cmd third = {
		.next = NULL,
		.root_redir = &first_output,
		.argv = {"wc", "-w", NULL}
	};

	t_cmd second = {
			.next = NULL,
			.root_redir = &first_output,
			.argv = {"grep", "d", NULL}
		};
	t_cmd	first = {
		.next = &second,
		.root_redir = &first_input,
		.argv = {"cat", NULL}
	};


	print_fd(1, "t_cmd:%i\n", count_cmds(&first));

	t_piping	piping_data;
	pid_t		pid;

	piping_data.cmd = &first;
	piping_data.num_cmds = 2; //count_cmds(&first);  // Number of commands specified in argv
	piping_data.prev_read = STDIN_FILENO;
	piping_data.i = 0;

	//Chekc HereDoc start
	check_heredoc(piping_data);
	//Chekc HereDoc End
	
	piping_data.cmd = &first;
	while (piping_data.i < piping_data.num_cmds)
	{
		create_pipe(piping_data.i, piping_data.num_cmds, &piping_data.pipefd);
		pid = fork();
		if (pid == -1) 
			pipex_error(1, "fork error", 1, errno);
		else if (pid == 0)
			child(envp, &piping_data, state);
		else
			parent(&piping_data);
        piping_data.i++;
		if (piping_data.cmd->next != NULL)
			piping_data.cmd = piping_data.cmd->next;
	}

	 //Wait for all child processes to finish
	while (piping_data.num_cmds > 0)
	{
		int status;
		wait(&status);
		piping_data.num_cmds--;
	}
}
