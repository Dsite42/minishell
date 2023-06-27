/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 16:39:25 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/27 21:21:20 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "../parser/parser.h"

#include <stdlib.h>
#include <stdarg.h>

size_t	ms_iter_words(void *context_ptr, t_slice slice,
			void (*callback)(void *context_ptr, t_slice word),
			int *p_trailing_space)
{
	t_slice	word;
	size_t	count;
	int		trailing_space;

	count = 0;
	trailing_space = 0;
	while (slice.size > 0)
	{
		slice = trim_left(slice, begin_space, NULL);
		if (slice.size == 0)
		{
			trailing_space = 1;
			break ;
		}
		split_once(slice, begin_space, &word, &slice);
		if (word.size > 0)
		{
			if (callback != NULL)
				callback(context_ptr, word);
			count++;
		}
	}
	if (p_trailing_space != NULL)
		*p_trailing_space = trailing_space;
	return (count);
}

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
