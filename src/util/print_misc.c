/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_print_misc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 11:03:45 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 18:12:28 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util_print.h"

int	print_pointer(int fd, int *p_total_length, void *address)
{
	unsigned long	number;
	char			buffer[18];
	char			*pointer;

	number = (unsigned long) address;
	pointer = &buffer[sizeof(buffer)];
	while (number || pointer == &buffer[sizeof(buffer)])
	{
		*(--pointer) = "0123456789abcdef"[number % 16];
		number /= 16;
	}
	*(--pointer) = 'x';
	*(--pointer) = '0';
	return (print_write(fd, p_total_length, pointer,
			&buffer[sizeof(buffer)] - pointer));
}

int	print_int(int fd, int *p_total_length, int number)
{
	char			buffer[11];
	char			*pointer;
	unsigned int	unsigned_part;
	int				is_negative;

	is_negative = number < 0;
	unsigned_part = number;
	if (is_negative)
	{
		unsigned_part = ~unsigned_part + 1;
	}
	pointer = &buffer[sizeof(buffer)];
	while (unsigned_part || pointer == &buffer[sizeof(buffer)])
	{
		*(--pointer) = (unsigned_part % 10) + '0';
		unsigned_part /= 10;
	}
	if (is_negative)
	{
		*(--pointer) = '-';
	}
	return (print_write(fd, p_total_length, pointer,
			&buffer[sizeof(buffer)] - pointer));
}

int	print_uint(int fd, int *p_total_length, unsigned int number)
{
	char	buffer[10];
	char	*pointer;

	pointer = &buffer[sizeof(buffer)];
	while (number || pointer == &buffer[sizeof(buffer)])
	{
		*(--pointer) = (number % 10) + '0';
		number /= 10;
	}
	return (print_write(fd, p_total_length, pointer,
			&buffer[sizeof(buffer)] - pointer));
}

int	print_hex(int fd, int *p_total_length, unsigned int number, int uppercase)
{
	char		buffer[16];
	char		*pointer;
	const char	*conversion_table;

	pointer = &buffer[sizeof(buffer)];
	if (uppercase)
	{
		conversion_table = "0123456789ABCDEF";
	}
	else
	{
		conversion_table = "0123456789abcdef";
	}
	while (number || pointer == &buffer[sizeof(buffer)])
	{
		*(--pointer) = conversion_table[number % 16];
		number /= 16;
	}
	return (print_write(fd, p_total_length, pointer,
			&buffer[sizeof(buffer)] - pointer));
}

int	print_slice(int fd, int *p_total_length, t_slice slice)
{
	return (print_write(fd, p_total_length, slice.data, slice.size));
}
