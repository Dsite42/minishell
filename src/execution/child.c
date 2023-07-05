/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 10:59:08 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/05 10:37:14 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "../builtin/builtin.h"
#include <stdlib.h>

void	child(char **argv, char **envp, t_piping *piping_data, t_state *state)
{
	int		fd_dup[2];
	char	*path_cmd;

	if (*piping_data->cmd->argv != NULL && is_builtin(piping_data->cmd->argv) \
			== 1 && piping_data->num_cmds == 1)
		exit(0);
	input_redirection(piping_data, fd_dup);
	if (output_redirection(piping_data, fd_dup) == -1)
		exit(1);
	if (*piping_data->cmd->argv != NULL && is_builtin(argv) == 1
		&& piping_data->num_cmds > 1)
		exit(run_builtin(argv, state));
	else if (*piping_data->cmd->argv != NULL && is_builtin(argv) == 0)
	{
		path_cmd = get_path_cmd(piping_data->cmd->argv, state);
		if (path_cmd == NULL)
			error_cmd_not_found(*argv);
		if (execve((const char *) path_cmd,
				piping_data->cmd->argv, envp) == -1)
			execution_error(1, *argv, 1, errno);
	}
	else
		exit(10);
}
