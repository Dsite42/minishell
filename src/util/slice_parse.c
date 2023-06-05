/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:22:12 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 21:07:33 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <limits.h>

static int	parse_digit(unsigned int *p_unsigned_part, t_slice *p_slice)
{
	unsigned int	digit;

	if (*p_unsigned_part != 0 && UINT_MAX / *p_unsigned_part < 10)
		return (0);
	*p_unsigned_part *= 10;
	digit = *(p_slice->data) - '0';
	if (UINT_MAX - *p_unsigned_part < digit)
		return (0);
	*p_unsigned_part += digit;
	*p_slice = advance(*p_slice);
	return (1);
}

static int	parse_signed(
	unsigned int unsigned_part, int is_negative, int *p_result)
{
	if (is_negative)
	{
		if (unsigned_part > ((unsigned int) INT_MAX + 1))
			return (0);
		*p_result = (int)(~unsigned_part + 1);
	}
	else
	{
		if (unsigned_part > (unsigned int) INT_MAX)
			return (0);
		*p_result = (int) unsigned_part;
	}
	return (1);
}

// Parses a number at the start of the slice
// If the slice doesn't start with a number, zero is returned
// NOTE: `p_slice` will only be updated if a valid number is found
int	parse_int(int *p_result, t_slice *p_slice)
{
	t_slice			slice;
	int				is_negative;
	unsigned int	unsigned_part;

	is_negative = 0;
	slice = *p_slice;
	if (consume(&slice, "-"))
		is_negative = 1;
	else
		consume(&slice, "+");
	unsigned_part = 0;
	while (slice.size > 0)
	{
		if (!ms_is_digit(*slice.data))
			break ;
		if (!parse_digit(&unsigned_part, &slice))
			return (0);
	}
	if (slice.data == p_slice->data)
		return (0);
	if (!parse_signed(unsigned_part, is_negative, p_result))
		return (0);
	*p_slice = slice;
	return (1);
}
