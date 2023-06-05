/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:00:26 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 20:48:46 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>

size_t	ms_ptrs_count(void *ptrs)
{
	void	**iter;

	iter = ptrs;
	while (*iter != NULL)
		iter++;
	return (iter - (void **) ptrs);
}

void	*ms_ptrs_free(void *ptrs)
{
	void	**iter;

	iter = ptrs;
	while (*iter != NULL)
	{
		free(*iter);
		iter++;
	}
	free(ptrs);
	return (NULL);
}

int	ms_is_digit(int input)
{
	return (input >= '0' && input <= '9');
}

int	ms_is_alpha(int input)
{
	return ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z'));
}

size_t	ms_str_length(const char *string)
{
	const char	*iter;

	iter = string;
	while (*iter != '\0')
		iter++;
	return (iter - string);
}
