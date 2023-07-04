/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 20:54:12 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state/state.h"
#include "parser/parser.h"
#include "execution/execution.h"

#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static t_result	handle_line(char *line, t_state *state, char **envp)
{
	t_result	result;
	t_cmd		*root_cmd;
	t_word		*root_word;

	add_history(line);
	result = words_from_slice(&root_word, slice0(line));
	if (result == S_OK)
	{
		result = cmds_from_words(root_word, &root_cmd, state);
		if (result == S_OK)
		{
			run_cmds(root_cmd, envp, state);
			cmd_clear(&root_cmd);
		}
		word_clear(&root_word);
	}
	free(line);
	return (result);
}

static void	handle_error(t_result result, t_state *state)
{
	if (result == S_OK)
		return ;
	if (state->error_info == NULL)
		print_fd(STDERR_FILENO, "minishell: %s\n", ms_result_to_string(result));
	else
	{
		print_fd(STDERR_FILENO, "minishell: %s: %s\n", state->error_info,
			ms_result_to_string(result));
		free(state->error_info);
		state->error_info = NULL;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state	state;
	char	*line;

	(void) argc;
	(void) argv;
	if (!state_init(&state, envp))
		return (1);
	tty_setup();
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break ;
		add_history(line);
		handle_error(handle_line(line, &state, envp), &state);
	}
	write(STDERR_FILENO, "exit\n", 5);
	state_drop(&state);
	return (0);
}
