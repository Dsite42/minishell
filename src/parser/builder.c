/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:43:48 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 15:53:39 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

static int	builder_append_group(
	t_builder *builder, unsigned int flags, t_slice slice)
{
	t_word	*new_word;

	if (!word_new(&new_word, flags, slice))
		return (0);
	if (builder->root_word == NULL)
		builder->root_word = new_word;
	if (builder->group_head_word != NULL)
		builder->group_head_word->next_group = new_word;
	builder->group_head_word = new_word;
	builder->chain_head_word = new_word;
	return (1);
}

static int	builder_append_chain(
	t_builder *builder, unsigned int flags, t_slice slice)
{
	t_word	*new_word;

	if (!word_new(&new_word, flags, slice))
		return (0);
	if (builder->root_word == NULL)
		builder->root_word = new_word;
	if (builder->group_head_word == NULL)
		builder->group_head_word = new_word;
	if (builder->chain_head_word != NULL)
		builder->chain_head_word->next_chain = new_word;
	builder->chain_head_word = new_word;
	return (1);
}

void	builder_group(t_builder *builder)
{
	builder->is_new_group = 1;
}

int	builder_append(t_builder *builder, unsigned int flags, t_slice slice)
{
	int	result;

	if (slice.size == 0)
		return (1);
	if (builder->is_new_group)
	{
		result = builder_append_group(builder, flags, slice);
		if (result)
			builder->is_new_group = 0;
		return (result);
	}
	return (builder_append_chain(builder, flags, slice));
}
