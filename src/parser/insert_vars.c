/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 17:54:20 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 18:25:55 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

// Starts a new group at the current chain
static void	start_new_group_leading(t_word **p_head_group,
		t_word **p_prev_chain, t_word **p_head_chain)
{
	if (*p_head_chain == *p_head_group)
		return ;
	if (*p_prev_chain != NULL)
		(*p_prev_chain)->next_chain = NULL;
	*p_prev_chain = NULL;
	(*p_head_chain)->next_group = (*p_head_group)->next_group;
	(*p_head_group)->next_group = *p_head_chain;
	*p_head_group = *p_head_chain;
}

// Starts a new group at the next chain
static void	start_new_group_trailing(t_word **p_head_group,
		t_word **p_prev_chain, t_word **p_head_chain)
{
	(void) p_head_group;
	(void) p_prev_chain;
	if ((*p_head_chain)->next_chain == NULL)
		return ;
	// FIXME: Not implemented
}

static t_result	expand_var_noquote(t_word **p_head_group, t_word **p_prev_chain,
		t_word **p_head_chain, t_slice slice)
{
	t_slice	first;
	size_t	count;

	split_once(slice, begin_space, &first, &slice);
	(*p_head_chain)->slice = first;
	while (slice.size > 0)
	{
		slice = trim_left(slice, begin_space, &count);
		if (slice.size == 0)
		{
			if (count > 0)
				start_new_group_trailing(p_head_group, p_prev_chain,
					p_head_chain);
			return (S_OK);
		}
		split_once(slice, begin_space, &first, &slice);
		// FIXME: Not implemented (insert another word)
	}
	return (S_OK);
}

static t_result	expand_var(t_word **p_head_group, t_word **p_prev_chain,
		t_word **p_head_chain, t_state *state)
{
	t_var	*var;
	t_slice	slice;
	size_t	count;

	var = vars_get(&state->root_var, (*p_head_chain)->slice);
	if (var == NULL)
		slice.size = 0;
	else
		slice = slice0(var->value);
	(*p_head_chain)->flags &= ~WORD_VAR;
	if ((*p_head_chain)->flags & WORD_QUOTE)
	{
		(*p_head_chain)->slice = slice;
		return (S_OK);
	}
	slice = trim_left(slice, begin_space, &count);
	if (count > 0)
		start_new_group_leading(p_head_group, p_prev_chain, p_head_chain);
	return (
		expand_var_noquote(p_head_group, p_prev_chain, p_head_chain, slice));
}

t_result	expand_vars(t_word *head_group, t_state *state)
{
	t_result	result;
	t_word		*prev_chain;
	t_word		*head_chain;

	while (head_group != NULL)
	{
		head_chain = head_group;
		prev_chain = NULL;
		while (head_chain != NULL)
		{
			if (head_chain->flags & WORD_VAR)
			{
				result = expand_var(
						&head_group, &prev_chain, &head_chain, state);
				if (result != S_OK)
					return (result);
			}
			prev_chain = head_chain;
			head_chain = head_chain->next_chain;
		}
		head_group = head_group->next_group;
	}
	return (S_OK);
}
