/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:58:05 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 15:25:44 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	is_minus_n(char *argv)
{
	int	i;

	i = 0;
	if (argv[i] == '-')
	{
		i++;
		while (argv[i] != '\0')
		{
			if (argv[i] != 'n')
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int	builtin_echo(int argc, char *argv[], int out_fd, t_state *state)
{
	int	index;
	int	print_newline;

	(void) state;
	print_newline = 1;
	index = 1;
	while (argc >= 2 && argv[index] != NULL && is_minus_n(argv[index]) == 1)
	{
		print_newline = 0;
		index++;
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
