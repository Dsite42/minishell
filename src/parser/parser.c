/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 01:18:23 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 20:38:58 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

// This function tries to parse an operator in regular state as an atomic word,
// it also tags operator words with the according flags to save string compares
static t_result	try_parse_operator(t_slice *p_remainder, t_builder *builder,
		size_t count, int *p_is_op)
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
	builder_group(builder);
	result = builder_append(builder, flags, first_part);
	builder_group(builder);
	*p_is_op = 1;
	return (result);
}

// This function is called when a dollar sign is consumed in regular state or
// double-quote state
static t_result	parse_variable(t_slice *p_remainder, t_builder *builder)
{
	t_slice	first_part;

	split_once(*p_remainder, begin_not_identifier, &first_part, p_remainder);
	if (first_part.size == 0)
	{
		consume(p_remainder, "$");
		return (builder_append(builder, 0, slice0("$")));
	}
	return (builder_append(builder, WORD_VAR, first_part));
}

// This function implements the double-quote state, its only way of getting
// called is when a double-quote is consumed in a state transition from regular
// state
static t_result	parse_double_quote(t_slice *p_remainder, t_builder *builder)
{
	t_slice		first_part;
	t_result	result;

	while (p_remainder->size > 0)
	{
		split_once(*p_remainder,
			begin_double_quote_split, &first_part, p_remainder);
		if (p_remainder->size == 0)
			return (E_DQTERM);
		result = builder_append(builder, 0, first_part);
		if (result != S_OK)
			return (result);
		if (consume(p_remainder, "\""))
			return (S_OK);
		else if (consume(p_remainder, "$"))
		{
			result = parse_variable(p_remainder, builder);
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
		t_slice *p_remainder, t_builder *builder, size_t count)
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
		return (builder_append(builder, 0, first_part));
	}
	else if (consume(p_remainder, "\""))
		return (parse_double_quote(p_remainder, builder));
	else if (consume(p_remainder, "$"))
		return (parse_variable(p_remainder, builder));
	result = try_parse_operator(p_remainder, builder, count, &is_op);
	if (result != S_OK)
		return (result);
	else if (!is_op && p_remainder->size > 0 && !begin_space(*p_remainder))
		return (E_BUG);
	return (S_OK);
}

// This function implements the regular state
t_result	word_chain_from_string(t_word **p_root_word, t_slice remainder)
{
	t_slice		first_part;
	t_builder	builder;
	t_result	result;
	size_t		count;

	ms_zero(&builder, sizeof(builder));
	while (remainder.size > 0)
	{
		count = split_once(remainder,
				begin_word_split, &first_part, &remainder);
		if (first_part.size > 0)
		{
			result = builder_append(&builder, 0, first_part);
			if (result != S_OK)
				return (builder_clean_return(&builder, result));
		}
		result = parse_word_split(&remainder, &builder, count);
		if (result != S_OK)
			return (builder_clean_return(&builder, result));
		remainder = trim_left(remainder, begin_space, &count);
		if (count > 0)
			builder_group(&builder);
	}
	*p_root_word = builder.root_word;
	return (S_OK);
}
