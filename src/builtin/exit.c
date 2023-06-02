/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:02:56 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/02 18:14:23 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdlib.h>

static int	isnumstr(char *str)
{
	while (*str)
	{
		if (!ms_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	is_spmn(const char *str, int *minus)
{
	int		i;

	i = 0;
	while ((str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*minus = 1;
		i++;
	}
	return (i);
}

static int	ft_atoi(const char *nprt)
{
	int	start_n;
	int	minus;
	int	int_res;
	int	i;

	minus = 0;
	start_n = is_spmn (nprt, &minus);
	i = start_n;
	int_res = 0;
	while (nprt[i] >= '0' && nprt[i] <= '9' && nprt[i] != '\0')
		i++;
	i = 0;
	while (nprt[start_n] >= '0' && nprt[start_n] <= '9'
		&& nprt[start_n] != '\0')
	{
		int_res = (int_res * 10) + (nprt[start_n] - 48);
		i++;
		start_n++;
	}
	if (minus == 1)
		int_res = int_res * -1;
	return (int_res);
}

void	exit_with_argvs(char **argv, int out_fd, t_state *state)
{
	if (isnumstr(*argv))
	{
		vars_clr(&state->root_var);
		print_fd(out_fd, "exit\n");
		exit(ft_atoi(*argv));
	}
	else
	{
		vars_clr(&state->root_var);
		print_fd(out_fd,
			"exit\nminishell: exit: %s: numeric argument required\n", *argv);
		exit(255);
	}
}

int	builtin_exit(int argc, char *argv[], int out_fd, t_state *state)
{
	argv++;
	argc = ms_arr_len(argv);
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
		exit_with_argvs(argv, out_fd, state);
	return (0);
}
