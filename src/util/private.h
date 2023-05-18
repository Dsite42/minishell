/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 18:09:11 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/18 16:09:44 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_H
# define PRIVATE_H

# include "util.h"

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int	print_write(int fd, int *p_total_length, const char *buffer, size_t length);
int	print_pointer(int fd, int *p_total_length, void *address);
int	print_int(int fd, int *p_total_length, int number);
int	print_uint(int fd, int *p_total_length, unsigned int number);
int	print_hex(int fd, int *p_total_length, unsigned int number, int uppercase);
int	print_slice(int fd, int *p_total_length, t_slice slice);

#endif // !PRIVATE_H