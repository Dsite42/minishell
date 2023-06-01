/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:14:45 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 22:59:12 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

size_t	begin_space(t_slice slice)
{
	if (slice.size == 0)
		return (0);
	return (*slice.data == ' ' || *slice.data == '\n' || *slice.data == '\r'
		|| *slice.data == '\t' || *slice.data == '\v' || *slice.data == '\f');
}

size_t	begin_not_identifier(t_slice slice)
{
	if (slice.size == 0)
		return (1);
	return ((*slice.data < 'a' || *slice.data > 'z')
		&& (*slice.data < 'A' || *slice.data > 'Z') && *slice.data != '_');
}

size_t	begin_word_split(t_slice slice)
{
	size_t	length;

	if (slice.size == 0)
		return (0);
	length = slice_str_begin(slice, "<<");
	if (length != 0)
		return (length);
	length = slice_str_begin(slice, ">>");
	if (length != 0)
		return (length);
	return (begin_space(slice) || *slice.data == '\'' || *slice.data == '"'
		|| *slice.data == '$' || *slice.data == '<' || *slice.data == '>'
		|| *slice.data == '|');
}

size_t	begin_single_quote_split(t_slice slice)
{
	if (slice.size == 0)
		return (0);
	return (*slice.data == '\'');
}

size_t	begin_double_quote_split(t_slice slice)
{
	if (slice.size == 0)
		return (0);
	return (*slice.data == '"' || *slice.data == '$');
}
