/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:43:48 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/23 01:38:44 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

static t_result	word_builder_append_group(
	t_word_builder *self, unsigned int flags, t_slice slice)
{
	t_word	*new_word;

	if (!word_new(&new_word, flags, slice))
		return (E_NOMEM);
	if (self->root_word == NULL)
		self->root_word = new_word;
	if (self->group_head_word != NULL)
		self->group_head_word->next_group = new_word;
	self->group_head_word = new_word;
	self->chain_head_word = new_word;
	return (S_OK);
}

static t_result	word_builder_append_chain(
	t_word_builder *self, unsigned int flags, t_slice slice)
{
	t_word	*new_word;

	if (!word_new(&new_word, flags, slice))
		return (E_NOMEM);
	if (self->root_word == NULL)
		self->root_word = new_word;
	if (self->group_head_word == NULL)
		self->group_head_word = new_word;
	if (self->chain_head_word != NULL)
		self->chain_head_word->next_chain = new_word;
	self->chain_head_word = new_word;
	return (S_OK);
}

void	word_builder_group(t_word_builder *self)
{
	self->is_new_group = 1;
}

t_result	word_builder_append(t_word_builder *builder,
		unsigned int flags, t_slice slice, int ignore_empty)
{
	t_result	result;

	if (ignore_empty && slice.size == 0)
		return (S_OK);
	if (builder->is_new_group)
	{
		result = word_builder_append_group(builder, flags, slice);
		if (result == S_OK)
			builder->is_new_group = 0;
		return (result);
	}
	return (word_builder_append_chain(builder, flags, slice));
}

t_result	word_builder_clean_return(t_word_builder *self, t_result result)
{
	words_clr(&self->root_word);
	return (result);
}
