/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:36:07 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/30 15:47:26 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state/state.h"
#include "parser/parser.h"
#include "builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>

void	run_cmds(t_cmd *root_cmd, char **envp, t_state *state);

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

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	t_result	result;
	char		*line;

	(void) argc;
	(void) argv;
	state.saved_STDOUT_FILENO = dup(1);
	if (!vars_from_envp(envp, &state.root_var))
		return (1);
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break ;
		add_history(line);
		result = handle_line(line, &state, envp);
		if (result != S_OK)
			print_fd(STDERR_FILENO, "Error code %i while parsing\n", result);
	}
	state_drop(&state);
	return (0);
}
