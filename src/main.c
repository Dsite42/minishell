/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/23 01:22:14 by jsprenge         ###   ########.fr       */
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

#define FLAGS "VO---D"

static void	dump_words(t_word *head_group)
{
	t_word		*head_chain;
	const char	*join_prefix;
	char		flag_prefix[sizeof(FLAGS)];

	while (head_group != NULL)
	{
		head_chain = head_group;
		while (head_chain != NULL)
		{
			ms_copy(flag_prefix, FLAGS, sizeof(FLAGS));
			for (unsigned int bit = 0; bit < sizeof(FLAGS) - 1; bit++)
				if ((head_chain->flags & (1 << bit)) == 0 && (bit < 2 || bit > 4))
					flag_prefix[bit] = '-';
			if ((head_chain->flags & WORD_OP_MASK) == WORD_OP_PIPE)
				ms_copy(&flag_prefix[2], "(P)", 3);
			if ((head_chain->flags & WORD_OP_MASK) == WORD_OP_READ)
				ms_copy(&flag_prefix[2], "(R)", 3);
			if ((head_chain->flags & WORD_OP_MASK) == WORD_OP_WRITE)
				ms_copy(&flag_prefix[2], "(W)", 3);
			if ((head_chain->flags & WORD_OP_MASK) == WORD_OP_APPEND)
				ms_copy(&flag_prefix[2], "(A)", 3);
			if ((head_chain->flags & WORD_OP_MASK) == WORD_OP_HEREDOC)
				ms_copy(&flag_prefix[2], "(H)", 3);
			if (head_chain == head_group && head_group->next_chain == NULL)
			{
				print_fd(STDOUT_FILENO, "  [%s] '%a'\n", flag_prefix, head_chain->slice);
				break ;
			}
			if (head_chain == head_group)
				join_prefix = "╭ ";
			else if (head_chain->next_chain == NULL)
				join_prefix = "╰ ";
			else
				join_prefix = "│ ";
			print_fd(STDOUT_FILENO, "%s[%s] '%a'\n", join_prefix, flag_prefix, head_chain->slice);
			head_chain = head_chain->next_chain;
		}
		head_group = head_group->next_group;
	}
}

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
	dump_words(root_word);
	words_clr(&root_word);
	free(line);
	if (new_argv == NULL)
		return (E_NOMEM);
	//run_builtin(new_argv, state);
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
