/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 16:06:44 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/05 13:50:09 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

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

static int	open_error(t_piping *piping_data)
{
	execution_error(0, piping_data->cmd->root_redir->name, 1, errno);
	return (-1);
}

static int	dup_write_append(t_piping *piping_data, int *fd_dup)
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
				return (open_error(piping_data));
			fd_dup[1] = dup2(fd_outfile, STDOUT_FILENO);
			if (fd_dup[1] == -1)
				execution_error(1, "dup_write_append_output_1 error", 1, errno);
			close(piping_data->pipefd[0]);
			close(fd_outfile);
		}
		piping_data->cmd->root_redir = piping_data->cmd->root_redir->next;
	}
	return (0);
}

int	output_redirection(t_piping *piping_data, int *fd_dup)
{
	if (is_write_or_append(piping_data->cmd->root_redir))
	{
		if (dup_write_append(piping_data, fd_dup) == -1)
			return (-1);
	}
	else if (piping_data->i < piping_data->num_cmds - 1)
	{
		fd_dup[1] = dup2(piping_data->pipefd[1], STDOUT_FILENO);
		if (fd_dup[1] == -1)
			execution_error(1, "dup_output_2 error", 1, errno);
		close(piping_data->pipefd[0]);
		close(piping_data->pipefd[1]);
	}
	return (0);
}
