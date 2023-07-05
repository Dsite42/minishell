/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:59:22 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 12:58:53 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"

#include <unistd.h>
#include <stdlib.h>

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
	if (!state_set_exit_code(state, 0))
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

int	state_set_exit_code(t_state *state, int exit_code)
{
	int		result;
	char	*string;

	string = ms_int_to_str(exit_code);
	if (string == NULL)
		return (0);
	result = vars_set(&state->root_var, slice0("?"), slice0(string)) != NULL;
	free(string);
	return (result);
}
