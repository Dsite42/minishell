/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms_part2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 13:01:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/15 14:35:29 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <stdlib.h>

char	*ms_two_strjoin(char *str1, char *str2, char *sep)
{
	char	*joined;
	char	*joined_start;
	int		len_join;

	len_join = ms_str_length(str1) + ms_str_length(str2)
		+ ms_str_length(sep) + 1;
	joined = (char *) malloc(len_join * sizeof(char));
	joined_start = joined;
	if (joined == NULL)
		return (NULL);
	ms_copy((void *)joined, (void *)str1, ms_str_length(str1));
	joined = joined + ms_str_length(str1);
	ms_copy(joined, sep, ms_str_length(sep));
	joined = joined + ms_str_length(sep);
	ms_copy(joined, str2, ms_str_length(str2));
	joined = joined + ms_str_length(str2);
	*(joined) = '\0';
	return (joined_start);
}
