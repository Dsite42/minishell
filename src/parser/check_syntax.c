/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 17:49:16 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 21:25:40 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

static t_result	check_missing_followup(t_word *head_group)
{
	if (head_group->flags & WORD_IS_OP && head_group->next_group == NULL)
	{
		if ((head_group->flags & WORD_OP_MASK) == WORD_OP_PIPE)
			return (E_SYNPIPE);
		return (E_SYNLF);
	}
	return (S_OK);
}

static t_result	check_double_operator(t_word *prev_group, t_word *head_group)
{
	if (prev_group == NULL || (prev_group->flags & WORD_IS_OP) == 0
		|| (head_group->flags & WORD_IS_OP) == 0)
		return (S_OK);
	if ((prev_group->flags & WORD_OP_MASK) == WORD_OP_PIPE)
		return (E_SYNPIPE);
	if ((head_group->flags & WORD_OP_MASK) == WORD_OP_PIPE)
		return (E_SYNPIPE);
	if ((head_group->flags & WORD_OP_MASK) == WORD_OP_READ)
		return (E_SYNREAD);
	if ((head_group->flags & WORD_OP_MASK) == WORD_OP_WRITE)
		return (E_SYNWRITE);
	if ((head_group->flags & WORD_OP_MASK) == WORD_OP_APPEND)
		return (E_SYNAPPEND);
	if ((head_group->flags & WORD_OP_MASK) == WORD_OP_HEREDOC)
		return (E_SYNHEREDOC);
	return (S_OK);
}

static t_result	push_var_info(
		t_word *head_chain, t_state *state, t_result original)
{
	if ((head_chain->flags & WORD_SOURCE) == 0)
		return (original);
	state->error_info = ms_join_slices(2, slice0("$"), head_chain->source);
	if (state->error_info == NULL)
		return (E_NOMEM);
	return (original);
}

static t_result	check_ambiguous_redirect(
		t_word *prev_group, t_word *head_chain, t_state *state)
{
	size_t	size;
	t_word	*start_word;

	start_word = head_chain;
	if (prev_group == NULL || (prev_group->flags & WORD_IS_OP) == 0)
		return (S_OK);
	size = 0;
	while (head_chain != NULL)
	{
		if (head_chain->flags & WORD_INSERT)
			return (push_var_info(head_chain, state, E_SYNAMB));
		size += head_chain->slice.size;
		head_chain = head_chain->next_chain;
	}
	if (size == 0)
		return (push_var_info(start_word, state, E_SYNAMB));
	return (S_OK);
}

t_result	check_syntax(t_word *head_group, t_state *state)
{
	t_result	result;
	t_word		*prev_group;

	if (head_group != NULL
		&& (head_group->flags & WORD_OP_MASK) == WORD_OP_PIPE)
		return (E_SYNPIPE);
	prev_group = NULL;
	while (head_group != NULL)
	{
		result = check_double_operator(prev_group, head_group);
		if (result != S_OK)
			return (result);
		result = check_missing_followup(head_group);
		if (result != S_OK)
			return (result);
		result = check_ambiguous_redirect(prev_group, head_group, state);
		if (result != S_OK)
			return (result);
		prev_group = head_group;
		head_group = head_group->next_group;
	}
	return (S_OK);
}
