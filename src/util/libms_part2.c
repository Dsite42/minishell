/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 16:39:25 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 20:46:03 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "../parser/parser.h"

#include <stdlib.h>
#include <stdarg.h>

static char	*join_slices_populate(va_list *list, char *buffer, size_t count)
{
	t_slice	slice;
	char	*current;

	if (buffer == NULL)
		return (NULL);
	current = buffer;
	while (count--)
	{
		slice = va_arg(*list, t_slice);
		ms_copy(current, slice.data, slice.size);
		current += slice.size;
	}
	*current = '\0';
	return (buffer);
}

char	*ms_join_slices(size_t count, ...)
{
	va_list	list;
	size_t	index;
	size_t	length;
	char	*buffer;

	va_start(list, count);
	index = 0;
	length = 0;
	while (index < count)
	{
		length += va_arg(list, t_slice).size;
		index++;
	}
	va_end(list);
	va_start(list, count);
	buffer = join_slices_populate(&list, malloc(length + 1), count);
	va_end(list);
	return (buffer);
}

char	*ms_mem_clone(const void *data, size_t size)
{
	void	*new_data;

	new_data = malloc(size);
	if (new_data == NULL)
		return (NULL);
	ms_copy(new_data, data, size);
	return (new_data);
}

char	*ms_int_to_str(int value)
{
	char			*iter;
	char			buffer[16];
	int				is_negative;
	int				is_first_time;
	unsigned int	unsigned_part;

	is_negative = 0;
	unsigned_part = (unsigned int) value;
	if (value < 0)
	{
		unsigned_part = ~unsigned_part + 1;
		is_negative = 1;
	}
	iter = &buffer[sizeof(buffer)];
	is_first_time = 1;
	*(--iter) = '\0';
	while (unsigned_part != 0 || is_first_time)
	{
		*(--iter) = '0' + unsigned_part % 10;
		unsigned_part /= 10;
		is_first_time = 0;
	}
	if (is_negative)
		*(--iter) = '-';
	return (ms_mem_clone(iter, &buffer[sizeof(buffer)] - iter));
}

const char	*ms_result_to_string(t_result result)
{
	if (result == E_BUG)
		return ("parser has invalid state");
	if (result == E_NOMEM)
		return ("out of memory");
	if (result == E_SQTERM)
		return ("unexpected EOF while looking for matching `''");
	if (result == E_DQTERM)
		return ("unexpected EOF while looking for matching `\"'");
	if (result == E_SYNPIPE)
		return ("syntax error near unexpected token `|'");
	if (result == E_SYNLF)
		return ("syntax error near unexpected token `newline'");
	if (result == E_SYNAMB)
		return ("ambiguous redirect");
	if (result == E_SYNREAD)
		return ("syntax error near unexpected token `<'");
	if (result == E_SYNWRITE)
		return ("syntax error near unexpected token `>'");
	if (result == E_SYNAPPEND)
		return ("syntax error near unexpected token `>>'");
	if (result == E_SYNHEREDOC)
		return ("syntax error near unexpected token `<<'");
	return ("unknown error");
}
