/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words_from_slice.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 01:18:23 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 14:37:53 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

// This function tries to parse an operator in regular state as an atomic word,
// it also tags operator words with the according flags to save string compares
static t_result	try_parse_operator(t_slice *p_remainder,
		t_word_builder *builder, size_t count, int *p_is_op)
{
	t_slice			first_part;
	t_result		result;
	unsigned int	flags;

	if (slice_str_begin(*p_remainder, "|"))
		flags = WORD_OP_PIPE;
	else if (slice_str_begin(*p_remainder, "<<"))
		flags = WORD_OP_HEREDOC;
	else if (slice_str_begin(*p_remainder, ">>"))
		flags = WORD_OP_APPEND;
	else if (slice_str_begin(*p_remainder, "<"))
		flags = WORD_OP_READ;
	else if (slice_str_begin(*p_remainder, ">"))
		flags = WORD_OP_WRITE;
	else
	{
		*p_is_op = 0;
		return (S_OK);
	}
	split_at(*p_remainder, count, &first_part, p_remainder);
	builder->is_new_group = 1;
	result = word_builder_append(builder, flags, first_part, 0);
	builder->is_new_group = 1;
	*p_is_op = 1;
	return (result);
}

// This function is called when a dollar sign is consumed in regular state or
// double-quote state
static t_result	parse_variable(t_slice *p_remainder, t_word_builder *builder,
		unsigned int flags)
{
	t_slice	first_part;

	if (consume(p_remainder, "?"))
		return (word_builder_append(builder, WORD_VAR | flags, slice0("?"), 0));
	split_once(*p_remainder, begin_not_identifier, &first_part, p_remainder);
	if (first_part.size == 0)
	{
		consume(p_remainder, "$");
		return (word_builder_append(builder, flags, slice0("$"), 0));
	}
	return (word_builder_append(builder, WORD_VAR | flags, first_part, 0));
}

// This function implements the double-quote state, its only way of getting
// called is when a double-quote is consumed in a state transition from regular
// state
static t_result	parse_double_quote(
	t_slice *p_remainder, t_word_builder *builder)
{
	t_slice		first_part;
	t_result	result;

	word_builder_ensure_one(builder, 0, WORD_QUOTE);
	while (p_remainder->size > 0)
	{
		split_once(*p_remainder,
			begin_double_quote_split, &first_part, p_remainder);
		if (p_remainder->size == 0)
			return (E_DQTERM);
		result = word_builder_append(builder, WORD_QUOTE, first_part, 1);
		if (result != S_OK)
			return (result);
		if (consume(p_remainder, "\""))
			return (word_builder_ensure_one(builder, 1, WORD_QUOTE));
		else if (consume(p_remainder, "$"))
		{
			result = parse_variable(p_remainder, builder, WORD_QUOTE);
			if (result != S_OK)
				return (result);
		}
		else
			return (E_BUG);
	}
	return (E_DQTERM);
}

// This function implements a state transition from regular state
static t_result	parse_word_split(
		t_slice *p_remainder, t_word_builder *builder, size_t count)
{
	t_slice		first_part;
	t_result	result;
	int			is_op;

	if (consume(p_remainder, "'"))
	{
		split_once(*p_remainder,
			begin_single_quote_split, &first_part, p_remainder);
		if (p_remainder->size == 0)
			return (E_SQTERM);
		*p_remainder = advance(*p_remainder);
		return (word_builder_append(builder, WORD_QUOTE, first_part, 0));
	}
	else if (consume(p_remainder, "\""))
		return (parse_double_quote(p_remainder, builder));
	else if (consume(p_remainder, "$"))
		return (parse_variable(p_remainder, builder, 0));
	result = try_parse_operator(p_remainder, builder, count, &is_op);
	if (result != S_OK)
		return (result);
	else if (!is_op && p_remainder->size > 0 && !begin_space(*p_remainder))
		return (E_BUG);
	return (S_OK);
}

// Parses the given slice to a word tree
t_result	words_from_slice(t_word **p_root_word, t_slice slice)
{
	t_slice			first_part;
	t_word_builder	builder;
	t_result		result;
	size_t			count;

	ms_zero(&builder, sizeof(builder));
	while (slice.size > 0)
	{
		count = split_once(slice,
				begin_word_split, &first_part, &slice);
		if (first_part.size > 0)
		{
			result = word_builder_append(&builder, 0, first_part, 0);
			if (result != S_OK)
				return (word_builder_clean_return(&builder, result));
		}
		result = parse_word_split(&slice, &builder, count);
		if (result != S_OK)
			return (word_builder_clean_return(&builder, result));
		slice = trim_left(slice, begin_space, &count);
		if (count > 0)
			builder.is_new_group = 1;
	}
	*p_root_word = builder.root_word;
	return (S_OK);
}
