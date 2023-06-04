/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:00:26 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 15:53:31 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>

int	ms_arr_len(char **arr)
{
	int	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	ms_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

size_t	ms_strlen(const char *string)
{
	const char	*iter;

	iter = string;
	while (*iter != '\0')
		iter++;
	return (iter - string);
}

int	ms_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

char	*ms_strchr(const char *s, int c)
{
	char	*cs;

	cs = (char *)s;
	c = c % 256;
	while (*cs != '\0')
	{
		if (*cs == c)
			return (cs);
		cs++;
	}
	if (*cs == '\0' && c == '\0')
		return (cs);
	return (0);
}

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
