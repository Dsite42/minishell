/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:56:58 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 15:34:13 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int	is_read_or_heredoc(t_redir *root_redir)
{
	while (root_redir != NULL)
	{
		if (root_redir->type == WORD_OP_READ
			|| root_redir->type == WORD_OP_HEREDOC)
			return (1);
		root_redir = root_redir->next;
	}
	return (0);
}

static void	dup_read(int *fd_dup, t_redir *head_redir)
{
	int	fd_infile;

	fd_infile = open(head_redir->name, O_RDONLY);
	if (fd_infile == -1)
		execution_error(1, "input:", 1, errno);
	fd_dup[0] = dup2(fd_infile, STDIN_FILENO);
	if (fd_dup[0] == -1)
		execution_error(1, "dup_infile_input_1 error", 1, errno);
	close(fd_infile);
}

void	dup_read_heredoc(t_piping *piping_data, int *fd_dup)
{
	int		fd_infile;
	t_redir	*head_redir;

	head_redir = piping_data->cmd->root_redir;
	while (head_redir != NULL)
	{
		if (head_redir->type == WORD_OP_READ)
			dup_read(fd_dup, head_redir);
		else if (head_redir->type == WORD_OP_HEREDOC
			&& head_redir->last_heredoc == 1)
		{
			fd_infile = head_redir->pipefd_heredoc[0];
			if (fd_infile == -1)
				execution_error(1, "input:", 1, errno);
			fd_dup[0] = dup2(fd_infile, STDIN_FILENO);
			if (fd_dup[0] == -1)
				execution_error(1, "dup_input_2 error", 1, errno);
			close(fd_infile);
		}
		head_redir = head_redir->next;
	}
}

void	input_redirection(t_piping *piping_data, int *fd_dup)
{
	if (is_read_or_heredoc(piping_data->cmd->root_redir))
		dup_read_heredoc(piping_data, fd_dup);
	else if (piping_data->prev_read != STDIN_FILENO)
	{
		fd_dup[0] = dup2(piping_data->prev_read, STDIN_FILENO);
		close(piping_data->prev_read);
		if (fd_dup[0] == -1)
		{
			print_fd(2, "i:%i\n", piping_data->i);
			execution_error(1, "dup_input_1 error", 1, errno);
		}
	}
}
