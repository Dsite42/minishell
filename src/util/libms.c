/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:00:26 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/20 11:57:55 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

size_t	ms_strlen(const char *string)
{
	const char	*iter;

	iter = string;
	while (*iter != '\0')
		iter++;
	return (iter - string);
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
