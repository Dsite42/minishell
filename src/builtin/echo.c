/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:58:05 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/20 17:35:54 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>
#include "../libft.h"

// ft_split is used to imitate the parser

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

int	builtin_echo(int argc, char *argv[], int out_fd, t_state *state)
{
	char	**splitted_argvs;
	int		n_flag;

	n_flag = 0;
	splitted_argvs = ft_split(argv[2], ' ');
	argc = arr_len(splitted_argvs);
	if (argc != 0)
	{
		if (ms_strlen(*splitted_argvs) == 2
			&& ft_strncmp(*splitted_argvs, "-n", 2) == 0)
			n_flag = 1;
	}
	if (argc == 0 || (argc == 1 && n_flag == 1))
	{
		if (n_flag != 1)
			print_fd(out_fd, "\n");
		return (0);
	}
	if (n_flag == 1)
		splitted_argvs++;
	while (*splitted_argvs)
	{
		print_fd(out_fd, "%s", *splitted_argvs);
		splitted_argvs++;
		if (*splitted_argvs)
			print_fd(out_fd, " ");
		else if (n_flag != 1)
			print_fd(out_fd, "\n");
	}
	return (0);
}
