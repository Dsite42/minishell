/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:43:48 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 03:17:15 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

int	builder_append_group(t_builder *builder, unsigned int flags, t_slice slice)
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

int	builder_append_chain(t_builder *builder, unsigned int flags, t_slice slice)
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
