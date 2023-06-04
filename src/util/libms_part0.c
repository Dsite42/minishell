/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:00:26 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 16:01:08 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>

int	ms_arr_len(char **arr)
{
	int	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	ms_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

size_t	ms_strlen(const char *string)
{
	const char	*iter;

	iter = string;
	while (*iter != '\0')
		iter++;
	return (iter - string);
}

int	ms_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

char	*ms_strchr(const char *s, int c)
{
	char	*cs;

	cs = (char *)s;
	c = c % 256;
	while (*cs != '\0')
	{
		if (*cs == c)
			return (cs);
		cs++;
	}
	if (*cs == '\0' && c == '\0')
		return (cs);
	return (0);
}
