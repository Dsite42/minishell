/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:02:56 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/20 16:50:39 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../libft.h"
#include <stdio.h>

// ft_split is used to imitate the parser

int	isnumstr(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	arr_len(char **arr)
{
	int	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	builtin_exit(int argc, char *argv[], int out_fd, t_state *state)
{
	char	**splitted_argvs;

	splitted_argvs = ft_split(argv[2], ' ');
	argc = arr_len(splitted_argvs);
	if (argc == 0)
	{
		vars_clr(&state->root_var);
		print_fd(out_fd, "exit\n");
		exit(state->exit_code);
	}
	else if (argc > 1)
	{
		print_fd(out_fd, "exit\nminishell: exit: too many arguments\n");
		return (1);
	}
	else if (argc == 1)
	{
		if (isnumstr(*splitted_argvs))
		{
			vars_clr(&state->root_var);
			print_fd(out_fd, "exit\n");
			exit(ft_atoi(*splitted_argvs));
		}
		else
		{
			vars_clr(&state->root_var);
			print_fd(out_fd, "exit\nminishell: exit: %s: numeric argument required\n", *splitted_argvs);
			exit(255);
		}
	}
}
