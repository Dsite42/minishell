/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:20:44 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 20:10:30 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "slice.h"
#include "util.h"

#include <stdlib.h>

t_slice	slice0(const char *string)
{
	t_slice	slice;

	slice.data = string;
	slice.size = ms_strlen(string);
	return (slice);
}

char	*slice_to_str(t_slice slice)
{
	char	*new_string;

	new_string = malloc(slice.size + 1);
	if (new_string == NULL)
		return (NULL);
	ms_memcpy(new_string, slice.data, slice.size);
	new_string[slice.size] = '\0';
	return (new_string);
}

int	slice_str_equal(t_slice slice, const char *string)
{
	size_t	index;

	index = 0;
	while (index < slice.size && string[index] != '\0')
	{
		if (slice.data[index] != string[index])
			return (0);
		index++;
	}
	if (index == slice.size && string[index] == '\0')
		return (1);
	return (0);
}
