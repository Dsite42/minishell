/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/03 20:16:36 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util/util.h"
#include "state/state.h"
#include "parser/parser.h"
#include "builtin/builtin.h"
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

#include <string.h>

static void	dump_argv(char **argv, t_state *state)
{
	if (ms_strncmp(*argv, "export", 6) == 0)
	{
		builtin_export(0, argv, 0, state);
	}
	if (ms_strncmp(*argv, "env", 3) == 0)
	{
		builtin_env(0, argv, 0, state);
	}
	if (ms_strncmp(*argv, "echo", 4) == 0)
	{
		builtin_echo(0, argv, 0, state);
	}
	if (ms_strncmp(*argv, "exit", 4) == 0)
	{
		builtin_exit(0, argv, 0, state);
	}
	if (ms_strncmp(*argv, "cd", 2) == 0)
	{
		builtin_cd(0, argv, 0, state);
	}
	if (ms_strncmp(*argv, "unset", 5) == 0)
	{
		//builtin_unset(0, argv, 0, state);
	}

  	//size_t	index;
	//index = 0;
	//while (argv[index] != NULL)
	//{
	//	print_fd(STDOUT_FILENO, "%u: %s\n", index, argv[index]);
	//	index++;
	//}
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
	dump_argv(new_argv, state);
	free_pointers(new_argv);
	return (S_OK);
}

// ctrl-\ does anyway nothing.
void	handle_signals(int sig)
{
	(void)sig;
	if (sig == SIGINT)
		write(0, "\nminishell> ", 12);
	else if (sig == SIGTSTP)
	{
		//vars_clr(&state.root_var); should be state as global?
		rl_clear_history();
		exit(0);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	t_result	result;
	char		*line;
	struct sigaction	sa;

	(void) argc;
	(void) argv;
	if (!vars_from_envp(envp, &state.root_var))
		return (1);
	sa.sa_handler = &handle_signals;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
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
	vars_clr(&state.root_var);
	return (0);
}
