/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:59:11 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/18 16:02:56 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

int	print_write(int fd, int *p_total_length, const char *buffer, size_t length)
{
	ssize_t	bytes_written;

	bytes_written = write(fd, buffer, length);
	if (bytes_written < 0)
	{
		*p_total_length = -1;
		return (-1);
	}
	*p_total_length += bytes_written;
	return (0);
}

static int	print_character(int fd, int *p_total_length, const char character)
{
	return (print_write(fd, p_total_length, &character, 1));
}

int	print_string(int fd, int *p_total_length, const char *string)
{
	const char	*iterator;

	if (string == NULL)
		return (print_string(fd, p_total_length, "(null)"));
	iterator = string;
	while (*iterator)
		iterator++;
	return (print_write(fd, p_total_length, string, iterator - string));
}

static int	print_format(int fd, int *p_total_length, va_list *list,
		const char **p_format)
{
	char	cmd;

	cmd = **p_format;
	if (cmd == '\0')
		return (0);
	(*p_format)++;
	if (cmd == 'c')
		return (print_character(fd, p_total_length, va_arg(*list, int)));
	if (cmd == 's')
		return (print_string(fd, p_total_length, va_arg(*list, const char *)));
	if (cmd == 'p')
		return (print_pointer(fd, p_total_length, va_arg(*list, void *)));
	if (cmd == 'd' || cmd == 'i')
		return (print_int(fd, p_total_length, va_arg(*list, int)));
	if (cmd == 'u')
		return (print_uint(fd, p_total_length, va_arg(*list, unsigned int)));
	if (cmd == 'x' || cmd == 'X')
		return (print_hex(fd, p_total_length, va_arg(*list, unsigned int),
				cmd == 'X'));
	if (cmd == '%')
		return (print_character(fd, p_total_length, '%'));
	if (cmd == 'a')
		return (print_slice(fd, p_total_length, va_arg(*list, t_slice)));
	(*p_format)--;
	return (0);
}

int	print_fd(int fd, const char *format, ...)
{
	va_list		list;
	const char	*begin;
	int			total_length;

	va_start(list, format);
	begin = format;
	total_length = 0;
	while (*format)
	{
		if (*format == '%')
		{
			if (print_write(fd, &total_length, begin, format - begin) < 0)
				break ;
			format++;
			if (print_format(fd, &total_length, &list, &format) < 0)
				break ;
			begin = format;
		}
		else
			format++;
	}
	va_end(list);
	if (begin != format && total_length >= 0)
		print_write(fd, &total_length, begin, format - begin);
	return (total_length);
}
