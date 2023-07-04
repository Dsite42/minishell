/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:31:16 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 15:39:28 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../state/state.h"
#include "../parser/parser.h"
#include <stdlib.h>
#include <string.h>

void	execution_error(int shall_exit, char *message,
			int isstrerror, int exit_code)
{
	if (isstrerror == 1)
		print_fd(2, "minishell: %s %s\n", message, strerror(exit_code));
	else
		print_fd(2, "minishell: %s\n", message);
	if (shall_exit == 1)
		exit(exit_code);
}

void	error_cmd_not_found(char *cmd)
{
	print_fd(2, "minishell: %s: command not found\n", cmd);
	exit(127);
}
