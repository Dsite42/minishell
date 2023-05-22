/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:26:53 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 01:48:16 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

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
		if (!predicate(slice.data[index]))
			break ;
		index++;
	}
	slice.data += index;
	slice.size -= index;
	return (slice);
}

int	consume(t_slice *p_slice, char predicate)
{
	if (p_slice->size > 0 && p_slice->data[0] == predicate)
	{
		p_slice->data++;
		p_slice->size--;
		return (1);
	}
	return (0);
}
