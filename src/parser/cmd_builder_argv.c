/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder_argv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 20:47:32 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/30 15:10:52 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

// Iterates over all words which aren't redirection-related until the word tree
// ends or a WORD_OP_PIPE is reached
static t_result	iter_only_args(t_cmd_builder *self, t_word *head_group,
	t_result (*callback) (t_cmd_builder *self, int is_group, t_word *word))
{
	t_result	result;
	t_word		*head_chain;

	result = S_OK;
	while (head_group != NULL && result == S_OK
		&& (head_group->flags & WORD_OP_MASK) != WORD_OP_PIPE)
	{
		if (head_group->flags & WORD_IS_OP)
		{
			head_group = head_group->next_group;
			if (head_group == NULL)
				break ;
			head_group = head_group->next_group;
			if (head_group == NULL)
				break ;
		}
		head_chain = head_group;
		while (head_chain != NULL && result == S_OK)
		{
			result = callback(self, head_group == head_chain, head_chain);
			head_chain = head_chain->next_chain;
		}
		head_group = head_group->next_group;
	}
	return (result);
}

// This is used with iter_only_args to count the number of arguments in argv
static t_result	argv_count(t_cmd_builder *self, int is_group, t_word *word)
{
	(void) word;
	if (is_group)
		self->size++;
	return (S_OK);
}

// This is used with iter_only_args to allocate each argument in argv
static t_result	argv_alloc(t_cmd_builder *self, int is_group, t_word *word)
{
	char	*new_buffer;

	if (is_group)
		self->size = 0;
	self->size += word->slice.size;
	if (word->next_chain == NULL)
	{
		new_buffer = malloc(self->size + 1);
		if (new_buffer == NULL)
			return (E_NOMEM);
		self->argv[self->index++] = new_buffer;
		return (S_OK);
	}
	return (S_OK);
}

// This is used with iter_only_args to fill each allocated argument in argv
static t_result	argv_copy(t_cmd_builder *self, int is_group, t_word *word)
{
	if (is_group)
		self->iter = self->argv[self->index++];
	ms_copy(self->iter, word->slice.data, word->slice.size);
	self->iter += word->slice.size;
	if (word->next_chain == NULL)
		*self->iter = '\0';
	return (S_OK);
}

// Takes the command root of a word tree and builds an argv out of it
t_result	cmd_builder_build_argv(t_cmd_builder *self, t_word *root_word)
{
	t_result	result;

	self->size = 0;
	iter_only_args(self, root_word, argv_count);
	self->argv = ms_zero_alloc(sizeof(char *), self->size + 1);
	if (self->argv == NULL)
		return (E_NOMEM);
	self->index = 0;
	result = iter_only_args(self, root_word, argv_alloc);
	if (result != S_OK)
	{
		ms_ptrs_free(self->argv);
		return (result);
	}
	self->index = 0;
	iter_only_args(self, root_word, argv_copy);
	return (S_OK);
}
