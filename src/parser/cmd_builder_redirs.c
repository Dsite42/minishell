/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder_redirs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 14:05:51 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 21:22:25 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

// Creates a non-heredoc redirection
static t_result	create_regular_redir(t_word *head_chain,
		t_redir **p_new_redir, unsigned int flags, size_t length)
{
	char	*iter;
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir) + length + 1);
	if (new_redir == NULL)
		return (E_NOMEM);
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
	*p_new_redir = new_redir;
	return (S_OK);
}

// Creates a redirection for the heredoc operator
static t_result	create_heredoc_redir(t_word *head_chain, t_redir **p_new_redir)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir) + 1 + head_chain->source.size + 1);
	if (new_redir == NULL)
		return (E_NOMEM);
	new_redir->type = WORD_OP_HEREDOC;
	new_redir->next = NULL;
	new_redir->name[0] = '$';
	ms_copy(&new_redir->name[1], head_chain->source.data,
		head_chain->source.size);
	new_redir->name[1 + head_chain->source.size] = '\0';
	*p_new_redir = new_redir;
	return (S_OK);
}

// Appends a redirection to the given list of redirections
static void	append_redir(
		t_redir *new_redir, t_redir **p_root_redir, t_redir **p_head_redir)
{
	if (*p_root_redir == NULL)
	{
		*p_root_redir = new_redir;
		*p_head_redir = new_redir;
		return ;
	}
	(*p_head_redir)->next = new_redir;
	*p_head_redir = new_redir;
}

// Creates a new t_redir structure and appends it to the given list
static t_result	push_redir(t_word *root_chain,
		t_redir **p_root_redir, t_redir **p_head_redir, unsigned int flags)
{
	t_result	result;
	size_t		length;
	t_redir		*new_redir;
	t_word		*head_chain;

	if ((flags & WORD_OP_MASK) == WORD_OP_HEREDOC
		&& root_chain->flags & WORD_SOURCE)
		result = create_heredoc_redir(root_chain, &new_redir);
	else
	{
		length = 0;
		head_chain = root_chain;
		while (head_chain != NULL)
		{
			length += head_chain->slice.size;
			head_chain = head_chain->next_chain;
		}
		result = create_regular_redir(root_chain, &new_redir, flags, length);
		if (result != S_OK)
			return (result);
	}
	append_redir(new_redir, p_root_redir, p_head_redir);
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
			result = push_redir(head_group->next_group,
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
