/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/12 17:54:25 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util/util.h"
#include "state/state.h"
#include "parser/parser.h"
#include "builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

#include <string.h>

static int	run_builtin(char **argv, t_state *state)
{
	int	count;

	count = (int) ms_ptrs_count(argv);
	if (ms_str_compare(argv[0], "cd", 0) == 0)
		return (builtin_cd(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "echo", 0) == 0)
		return (builtin_echo(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "env", 0) == 0)
		return (builtin_env(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "exit", 0) == 0)
		return (builtin_exit(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "export", 0) == 0)
		return (builtin_export(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "pwd", 0) == 0)
		return (builtin_pwd(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "unset", 0) == 0)
		return (builtin_unset(count, argv, STDOUT_FILENO, state));
	print_fd(STDERR_FILENO, "minishell: %s: command not found\n", argv[0]);
	return (127);
}

static t_result	handle_line(char *line, t_state *state)
{
	t_word		*root_word;
	char		**new_argv;
	t_result	result;

	result = word_chain_from_string(&root_word, slice0(line));
	if (result != S_OK)
	{
		free(line);
		return (result);
	}
	add_history(line);
	new_argv = argv_from_word_group(root_word, &state->root_var);
	words_clr(&root_word);
	free(line);
	if (new_argv == NULL)
		return (E_NOMEM);
	run_builtin(new_argv, state);
	ms_ptrs_free(new_argv);
	return (S_OK);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	t_result	result;
	char		*line;

	(void) argc;
	(void) argv;
	if (!vars_from_envp(envp, &state.root_var))
		return (1);
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break ;
		add_history(line);
		result = handle_line(line, &state);
		if (result != S_OK)
			print_fd(STDERR_FILENO, "Error code %i while parsing\n", result);
	}
	state_drop(&state);
	return (0);
}
