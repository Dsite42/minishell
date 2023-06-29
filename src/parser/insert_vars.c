/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 17:54:20 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 19:59:30 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

// Breaks the current word (head_chain) out of its chain into a new group,
// inheriting all following chained words
// This also sets p_head_group to the new group to prevent a group mismatch
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

// Breaks the following word (head_chain->next) out of its chain into a new
// group, inheriting all following chained words
// This does not modify p_head_chain because it must still be iterated over
static void	start_new_group_trailing(t_word **p_head_group,
		t_word **p_head_chain)
{
	t_word	*next_chain;

	next_chain = (*p_head_chain)->next_chain;
	if (next_chain == NULL)
		return ;
	(*p_head_chain)->next_chain = NULL;
	next_chain->next_group = (*p_head_group)->next_group;
	(*p_head_group)->next_group = next_chain;
}

// Splits the variable's content by whitespace and modifies the group and chain
// links to split formerly joined words if required, if multiple splits occurr,
// new words are added in-between
static t_result	insert_var_noquote(t_word **p_head_group, t_word **p_head_chain,
		t_slice slice)
{
	t_slice	first;
	size_t	count;
	t_word	*new_word;

	split_once(slice, begin_space, &first, &slice);
	(*p_head_chain)->slice = first;
	while (slice.size > 0)
	{
		slice = trim_left(slice, begin_space, &count);
		if (slice.size == 0)
		{
			if (count > 0)
				start_new_group_trailing(p_head_group, p_head_chain);
			return (S_OK);
		}
		split_once(slice, begin_space, &first, &slice);
		if (!word_new(&new_word, 0, first))
			return (E_NOMEM);
		new_word->next_chain = (*p_head_chain)->next_chain;
		new_word->next_group = (*p_head_group)->next_group;
		(*p_head_chain)->next_chain = NULL;
		(*p_head_group)->next_group = new_word;
	}
	return (S_OK);
}

// Obtains the content of a to-be-expanded variable and transfers it directly
// to the word's slice
// The way of transfer is depending on whether the word is in a quote or not
// - If it is, the whole variable can be used directly
// - If not, it needs special handling to match bash (see insert_var_noquote)
static t_result	insert_var(t_word **p_head_group, t_word **p_prev_chain,
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
		insert_var_noquote(p_head_group, p_head_chain, slice));
}

// Transforms WORD_VAR tokens in the given word tree into their expanded value
t_result	insert_vars(t_word *head_group, t_state *state)
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
				result = insert_var(
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
