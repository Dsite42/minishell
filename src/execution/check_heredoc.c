/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:09:06 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 17:06:12 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

static t_redir	*search_last_heredoc(t_redir **root_redir)
{
	t_redir	*last_heredoc;
	t_redir	*root_dir_start;

	root_dir_start = *root_redir;
	last_heredoc = NULL;
	while (*root_redir != NULL)
	{
		if ((*root_redir)->type == WORD_OP_HEREDOC)
			last_heredoc = *root_redir;
		*root_redir = (*root_redir)->next;
	}
	if (last_heredoc != NULL)
		last_heredoc->last_heredoc = 1;
	*root_redir = root_dir_start;
	return (last_heredoc);
}

static void	run_heredoc(t_piping *piping_data)
{
	char	*line;

	create_pipe(0, 2, &piping_data->cmd->root_redir->pipefd_heredoc);
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ms_str_compare(
				line, piping_data->cmd->root_redir->name, 0) == 0)
		{
			free(line);
			close(piping_data->cmd->root_redir->pipefd_heredoc[1]);
			break ;
		}
		else
			print_fd(piping_data->cmd->root_redir->pipefd_heredoc[1],
				"%s\n", line);
		free(line);
	}
}

void	check_heredoc(t_piping piping_data)
{
	t_redir	*root_dir_start;
	t_cmd	*cmd_start;

	cmd_start = piping_data.cmd;
	while (piping_data.cmd != NULL)
	{
		root_dir_start = piping_data.cmd->root_redir;
		search_last_heredoc(&piping_data.cmd->root_redir);
		while (piping_data.cmd->root_redir != NULL)
		{
			if (piping_data.cmd->root_redir->type == WORD_OP_HEREDOC)
			{
				run_heredoc(&piping_data);
			}
			piping_data.cmd->root_redir = piping_data.cmd->root_redir->next;
		}
		piping_data.cmd->root_redir = root_dir_start;
		piping_data.cmd = piping_data.cmd->next;
	}
	piping_data.cmd = cmd_start;
}
