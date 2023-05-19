/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:00:26 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 20:01:21 by jsprenge         ###   ########.fr       */
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
