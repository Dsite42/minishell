/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:58:05 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/04 21:51:31 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_echo(int argc, char *argv[], int out_fd, t_state *state)
{
	int	index;
	int	print_newline;

	(void) state;
	if (argc >= 2 && ms_str_compare("-n", argv[1], 0) == 0)
	{
		print_newline = 0;
		index = 2;
	}
	else
	{
		print_newline = 1;
		index = 1;
	}
	while (index < argc)
	{
		print_fd(out_fd, "%s", argv[index]);
		if (index != argc - 1)
			print_fd(out_fd, " ");
		index++;
	}
	if (print_newline)
		print_fd(out_fd, "\n");
	return (0);
}
