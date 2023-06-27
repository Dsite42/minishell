/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter_cmd_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:25:55 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/27 21:05:34 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// TODO: Check if command ends with operator which is invalid

static t_result	iter_inner(void *context_ptr, t_word *head_group,
	t_result (*callback)(void *context_ptr, int is_group, t_word *word))
{
	t_result	result;
	t_word		*head_chain;
	int			ignore_count;

	ignore_count = 0;
	head_chain = head_group;
	while (head_chain != NULL)
	{
		if (head_chain->flags & WORD_IS_OP)
			ignore_count = 2;
		if (ignore_count > 0)
			ignore_count--;
		else
		{
			result = callback(context_ptr,
					head_chain == head_group, head_chain);
			if (result != S_OK)
				return (result);
		}
		head_chain = head_chain->next_chain;
	}
	return (S_OK);
}

t_result	words_iter_cmd_args(void *context_ptr, t_word **p_head_group,
	t_result (*callback)(void *context_ptr, int is_group, t_word *word))
{
	t_result	result;
	t_word		*head_group;

	head_group = *p_head_group;
	while (head_group != NULL
		&& (head_group->flags & WORD_OP_MASK) != WORD_OP_PIPE)
	{
		result = iter_inner(context_ptr, head_group, callback);
		if (result != S_OK)
			return (result);
		head_group = head_group->next_group;
	}
	if (head_group == NULL)
		*p_head_group = NULL;
	else
		*p_head_group = head_group->next_group;
	return (S_OK);
}
