/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:00:48 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 16:01:18 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>

int	ms_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		diff_val;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
		{
			diff_val = (unsigned char)s1[i] - (unsigned char)s2[i];
			return (diff_val);
		}
		i++;
	}
	return (0);
}

void	*ms_memcpy(void *destination, const void *source, size_t size)
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

void	ms_bzero(void *destination, size_t size)
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

void	*ms_calloc(size_t size, size_t count)
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

void	*free_pointers(void *pointers)
{
	void	**iter;

	iter = pointers;
	while (*iter != NULL)
	{
		free(*iter);
		iter++;
	}
	free(pointers);
	return (NULL);
}
