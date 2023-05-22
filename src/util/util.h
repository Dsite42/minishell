/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 15:59:24 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 02:58:54 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

// libms.c
size_t	ms_strlen(const char *string);
void	*ms_memcpy(void *destination, const void *source, size_t size);
void	ms_bzero(void *destination, size_t size);
void	*ms_calloc(size_t size, size_t count);
void	*free_pointers(void *pointers);

// print_core.c
int		print_fd(int fd, const char *format, ...);

typedef struct s_slice
{
	const char	*data;
	size_t		size;
}	t_slice;

// slice_core.c
t_slice	advance(t_slice slice);
void	split_once(t_slice slice, int (*predicate)(char),
			t_slice *p_part0, t_slice *p_part1);
t_slice	trim_left(t_slice slice, int (*predicate)(char));

// slice_str.c
t_slice	slice0(const char *string);
char	*slice_to_str(t_slice slice);
int		slice_str_equal(t_slice slice, const char *string);

#endif // !UTIL_H