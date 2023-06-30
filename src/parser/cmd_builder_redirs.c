/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder_redirs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 14:05:51 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/30 15:17:33 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

// Populates a t_redir structure from the given data
static void	populate_redir(
		t_word *head_chain, t_redir *new_redir, unsigned int flags)
{
	char	*iter;

	new_redir->type = flags & WORD_OP_MASK;
	new_redir->next = NULL;
	iter = new_redir->name;
	while (head_chain != NULL)
	{
		ms_copy(iter, head_chain->slice.data, head_chain->slice.size);
		iter += head_chain->slice.size;
		head_chain = head_chain->next_chain;
	}
	*iter = '\0';
}

// Creates a new t_redir structure and appends it to the given list
static t_result	append_redir(t_word *root_chain,
		t_redir **p_root_redir, t_redir **p_head_redir, unsigned int flags)
{
	size_t	length;
	t_redir	*new_redir;
	t_word	*head_chain;

	length = 0;
	head_chain = root_chain;
	while (head_chain != NULL)
	{
		length += head_chain->slice.size;
		head_chain = head_chain->next_chain;
	}
	new_redir = malloc(sizeof(t_redir) + length + 1);
	if (new_redir == NULL)
		return (E_NOMEM);
	populate_redir(root_chain, new_redir, flags);
	if (*p_root_redir == NULL)
	{
		*p_root_redir = new_redir;
		*p_head_redir = new_redir;
		return (S_OK);
	}
	(*p_head_redir)->next = new_redir;
	*p_head_redir = new_redir;
	return (S_OK);
}

// Takes the command root of a word tree and builds t_redir structures out of it
t_result	cmd_builder_build_redirs(t_cmd_builder *self, t_word *root_word)
{
	t_result	result;
	t_word		*head_group;
	t_redir		*head_redir;

	head_group = root_word;
	while (head_group != NULL
			&& (head_group->flags & WORD_OP_MASK) != WORD_OP_PIPE)
	{
		if (head_group->flags & WORD_IS_OP)
		{
			result = append_redir(head_group->next_group,
					&self->root_redir, &head_redir, head_group->flags);
			if (result != S_OK)
				return (result);
			head_group = head_group->next_group;
			if (head_group == NULL)
				break ;
		}
		head_group = head_group->next_group;
	}
	return (S_OK);
}
