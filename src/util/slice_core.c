/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:26:53 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 22:33:56 by jsprenge         ###   ########.fr       */
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

size_t	split_once(t_slice slice, size_t (*predicate)(t_slice),
			t_slice *p_part0, t_slice *p_part1)
{
	t_slice	part0;
	t_slice	part1;
	size_t	amount;

	part0.data = slice.data;
	part0.size = 0;
	part1 = slice;
	amount = 0;
	while (part0.size < slice.size)
	{
		amount = predicate(part1);
		if (amount > 0)
			break ;
		part0.size++;
		part1.data++;
		part1.size--;
	}
	if (p_part0 != NULL)
		*p_part0 = part0;
	if (p_part1 != NULL)
		*p_part1 = part1;
	return (amount);
}

t_slice	trim_left(t_slice slice, size_t (*predicate)(t_slice), size_t *p_count)
{
	size_t	count;
	size_t	amount;

	count = 0;
	while (slice.size > 0)
	{
		amount = predicate(slice);
		if (amount == 0)
			break ;
		if (amount > slice.size)
			amount = slice.size;
		count += amount;
		slice.data += amount;
		slice.size -= amount;
	}
	if (p_count != NULL)
		*p_count = count;
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
