/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:01:19 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 16:01:19 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <unistd.h>

void	print_fd(int fd, const char *string)
{
	const char	*iter;

	iter = string;
	while (*iter != '\0')
		iter++;
	write(fd, string, iter - string);
}
