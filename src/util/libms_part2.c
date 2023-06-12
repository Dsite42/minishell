/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 16:39:25 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/12 16:59:04 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "../parser/parser.h"

size_t	ms_iter_words(void *context_ptr, t_slice slice,
			void (*callback)(void *context_ptr, t_slice word))
{
	t_slice	word;
	size_t	count;

	count = 0;
	while (slice.size > 0)
	{
		slice = trim_left(slice, begin_space, NULL);
		split_once(slice, begin_space, &word, &slice);
		if (word.size > 0)
		{
			if (callback != NULL)
				callback(context_ptr, word);
			count++;
		}
	}
	return (count);
}
