/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:20:44 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/15 18:20:09 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <string.h>
#include <stdlib.h>

t_slice	slice0(const char *string)
{
	t_slice	slice;

	slice.data = string;
	slice.size = strlen(string);
	return (slice);
}

char	*slice_to_str(t_slice slice)
{
	char	*new_string;

	new_string = malloc(slice.size + 1);
	if (new_string == NULL)
		return (NULL);
	memcpy(new_string, slice.data, slice.size);
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

t_slice	advance(t_slice slice)
{
	if (slice.size > 0)
	{
		slice.data++;
		slice.size--;
	}
	return (slice);
}

void	split_once(t_slice slice, int (*predicate)(char),
			t_slice *p_part0, t_slice *p_part1)
{
	t_slice	part0;
	t_slice	part1;

	part0.data = slice.data;
	part0.size = 0;
	while (part0.size < slice.size)
	{
		if (predicate(slice.data[part0.size]))
			break ;
		part0.size++;
	}
	part1.data = &slice.data[part0.size];
	part1.size = slice.size - part0.size;
	if (p_part0 != NULL)
		*p_part0 = part0;
	if (p_part1 != NULL)
		*p_part1 = part1;
}

t_slice	trim_left(t_slice slice, int (*predicate)(char))
{
	size_t	index;

	index = 0;
	while (index < slice.size)
	{
		if (predicate(slice.data[index]))
			break ;
	}
	slice.data += index;
	slice.size -= index;
	return (slice);
}
