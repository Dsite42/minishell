/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:00:26 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/19 21:37:01 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>

size_t	ms_strlen(const char *string)
{
	const char	*iter;

	iter = string;
	while (*iter != '\0')
		iter++;
	return (iter - string);
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
	while (index < count)
		new_memory[index++] = 0;
	return (new_memory);
}
