/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:28:41 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 23:35:56 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

int	word_new(t_word **p_new_word, unsigned int flags, t_slice slice)
{
	t_word	*new_word;

	new_word = malloc(sizeof(t_word));
	if (new_word == NULL)
		return (0);
	new_word->flags = flags;
	new_word->slice = slice;
	new_word->next_group = NULL;
	new_word->next_chain = NULL;
	*p_new_word = new_word;
	return (1);
}

void	word_clear(t_word **p_root_word)
{
	t_word	*group_head_word;
	t_word	*group_next_word;
	t_word	*chain_head_word;
	t_word	*chain_next_word;

	group_head_word = *p_root_word;
	while (group_head_word != NULL)
	{
		chain_head_word = group_head_word;
		group_next_word = group_head_word->next_group;
		while (chain_head_word != NULL)
		{
			chain_next_word = chain_head_word->next_chain;
			free(chain_head_word);
			chain_head_word = chain_next_word;
		}
		group_head_word = group_next_word;
	}
	*p_root_word = NULL;
}

void	cmd_clear(t_cmd **p_root_cmd)
{
	t_cmd	*head_cmd;
	t_cmd	*next_cmd;
	t_redir	*head_redir;
	t_redir	*next_redir;

	head_cmd = *p_root_cmd;
	while (head_cmd != NULL)
	{
		next_cmd = head_cmd->next;
		ms_ptrs_free(head_cmd->argv);
		head_redir = head_cmd->root_redir;
		while (head_redir != NULL)
		{
			next_redir = head_redir->next;
			free(head_redir);
			head_redir = next_redir;
		}
		free(head_cmd);
		head_cmd = next_cmd;
	}
	*p_root_cmd = NULL;
}
