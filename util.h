/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:15:52 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/15 18:17:44 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

typedef struct s_slice
{
	const char	*data;
	size_t		size;
}	t_slice;

t_slice	slice0(const char *string);
char	*slice_to_str(t_slice slice);
int		slice_str_equal(t_slice slice, const char *string);

t_slice	advance(t_slice slice);
void	split_once(t_slice slice, int (*predicate)(char),
			t_slice *p_part0, t_slice *p_part1);
t_slice	trim_left(t_slice slice, int (*predicate)(char));

#endif // !UTIL_H