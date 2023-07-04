/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:59:22 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 20:42:04 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"

#include <unistd.h>

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

int	state_init(t_state *state, char *envp[])
{
	state->error_info = NULL;
	state->root_var = NULL;
	state->saved_stdin = -1;
	state->saved_stdout = -1;
	state->saved_stdin = dup(STDIN_FILENO);
	if (state->saved_stdin < 0)
	{
		perror("state_init: dup(STDIN_FILENO) failed");
		return (state_drop(state), 0);
	}
	state->saved_stdout = dup(STDOUT_FILENO);
	if (state->saved_stdout < 0)
	{
		perror("state_init: dup(STDOUT_FILENO) failed");
		return (state_drop(state), 0);
	}
	if (!vars_from_envp(envp, &state->root_var))
		return (state_drop(state), 0);
	if (vars_set(&state->root_var, slice0("?"), slice0("0")) == NULL)
		return (state_drop(state), 0);
	return (1);
}

void	state_drop(t_state *state)
{
	if (state->saved_stdin != -1)
		close(state->saved_stdin);
	if (state->saved_stdout != -1)
		close(state->saved_stdout);
	vars_clr(&state->root_var);
	rl_clear_history();
}
