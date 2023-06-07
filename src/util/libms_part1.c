/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:00:48 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 21:27:13 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>

char	*ms_str_find(const char *string, int predicate)
{
	while (*string)
	{
		if (*string == (char) predicate)
		{
			return ((char *) string);
		}
		string++;
	}
	if ((char) predicate == '\0')
		return ((char *) string);
	return (NULL);
}

int	ms_str_compare(const char *string1, const char *string2, size_t limit)
{
	char	lhs;
	char	rhs;
	int		index;
	int		difference;
	size_t	original_limit;

	index = 0;
	original_limit = limit;
	while (original_limit == 0 || limit--)
	{
		lhs = string1[index];
		rhs = string2[index];
		index++;
		difference = ((unsigned char) lhs) - ((unsigned char) rhs);
		if (difference != 0)
			return (difference);
		if (lhs == '\0')
			return (0);
	}
	return (0);
}

void	*ms_copy(void *destination, const void *source, size_t size)
{
	size_t		index;
	const char	*source_bytes;
	char		*destination_bytes;

	index = 0;
	source_bytes = source;
	destination_bytes = destination;
	while (index < size)
	{
		destination_bytes[index] = source_bytes[index];
		index++;
	}
	return (destination);
}

void	ms_zero(void *destination, size_t size)
{
	size_t	index;
	char	*destination_bytes;

	index = 0;
	destination_bytes = destination;
	while (index < size)
	{
		destination_bytes[index] = 0;
		index++;
	}
}

void	*ms_zero_alloc(size_t size, size_t count)
{
	size_t	index;
	size_t	length;
	char	*new_memory;

	if (size > 0 && count > ((size_t) -1) / size)
		return (NULL);
	length = size * count;
	new_memory = malloc(length);
	if (new_memory == NULL)
		return (NULL);
	index = 0;
	while (index < length)
		new_memory[index++] = 0;
	return (new_memory);
}
