/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:59:08 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/05 14:48:15 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "../builtin/builtin.h"
#include <stdlib.h>

static int	child_inner(
	char **argv, char **envp, t_piping *piping_data, t_state *state)
{
	int		fd_dup[2];
	char	*path_cmd;

	if (*piping_data->cmd->argv != NULL && is_builtin(piping_data->cmd->argv) \
			== 1 && piping_data->num_cmds == 1)
		return (0);
	input_redirection(piping_data, fd_dup);
	if (output_redirection(piping_data, fd_dup) == -1)
		return (1);
	if (*piping_data->cmd->argv != NULL && is_builtin(argv) == 1
		&& piping_data->num_cmds > 1)
		return (run_builtin(argv, state));
	else if (*piping_data->cmd->argv != NULL && is_builtin(argv) == 0)
	{
		path_cmd = get_path_cmd(piping_data->cmd->argv, state);
		if (path_cmd == NULL)
			error_cmd_not_found(*argv);
		if (execve((const char *) path_cmd,
				piping_data->cmd->argv, envp) == -1)
		{
			execution_error(0, *argv, 1, errno);
			return (errno);
		}
	}
	return (10);
}

void	child(char **argv, char **envp, t_piping *piping_data, t_state *state)
{
	int	exit_code;

	exit_code = child_inner(argv, envp, piping_data, state);
	state_drop(state);
	exit(exit_code);
}
