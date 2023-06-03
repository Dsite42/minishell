/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:58:05 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/02 17:39:23 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	is_print_no_new_line(int argc, char **argv, int *n_flag)
{
	if (argc != 0)
	{
		if (ms_strlen(*argv) == 2
			&& ms_strncmp(*argv, "-n", 2) == 0)
			*n_flag = 1;
	}
}

int	builtin_echo(int argc, char *argv[], int out_fd, t_state *state)
{
	int				n_flag;

	(void) state;
	n_flag = 0;
	argv++;
	argc = ms_arr_len(argv);
	is_print_no_new_line(argc, argv, &n_flag);
	if (argc == 0 || (argc == 1 && n_flag == 1))
	{
		if (n_flag != 1)
			print_fd(out_fd, "\n");
		return (0);
	}
	if (n_flag == 1)
		argv++;
	while (*argv)
	{
		print_fd(out_fd, "%s", *argv);
		argv++;
		if (*argv)
			print_fd(out_fd, " ");
		else if (n_flag != 1)
			print_fd(out_fd, "\n");
	}
	return (0);
}
