/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 22:32:00 by jsprenge         ###   ########.fr       */
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

// Temporary include
#include "parser/private.h"

#define FLAGS "VO---Q"

static void	dump_flags(char flag_prefix[sizeof(FLAGS)], unsigned int flags)
{
	unsigned int	bit;

	ms_copy(flag_prefix, FLAGS, sizeof(FLAGS));
	bit = 0;
	while (bit < sizeof(FLAGS) - 1)
	{
		if ((flags & (1 << bit)) == 0 && (bit < 2 || bit > 4))
			flag_prefix[bit] = '-';
		bit++;
	}
	if ((flags & WORD_OP_MASK) == WORD_OP_PIPE)
		ms_copy(&flag_prefix[2], "(P)", 3);
	if ((flags & WORD_OP_MASK) == WORD_OP_READ)
		ms_copy(&flag_prefix[2], "(R)", 3);
	if ((flags & WORD_OP_MASK) == WORD_OP_WRITE)
		ms_copy(&flag_prefix[2], "(W)", 3);
	if ((flags & WORD_OP_MASK) == WORD_OP_APPEND)
		ms_copy(&flag_prefix[2], "(A)", 3);
	if ((flags & WORD_OP_MASK) == WORD_OP_HEREDOC)
		ms_copy(&flag_prefix[2], "(H)", 3);
}

static void	dump_word(t_word *head_group, t_word *head_chain)
{
	const char	*join_prefix;
	char		flag_prefix[sizeof(FLAGS)];

	dump_flags(flag_prefix, head_chain->flags);
	if (head_chain == head_group && head_group->next_chain == NULL)
	{
		print_fd(STDOUT_FILENO, "  [%s] '%a'\n", flag_prefix, head_chain->slice);
		return ;
	}
	if (head_chain == head_group)
		join_prefix = "╭ ";
	else if (head_chain->next_chain == NULL)
		join_prefix = "╰ ";
	else
		join_prefix = "│ ";
	print_fd(STDOUT_FILENO, "%s[%s] '%a'\n",
		join_prefix, flag_prefix, head_chain->slice);
}

static void	dump_words(t_word *head_group)
{
	t_word	*head_chain;

	while (head_group != NULL)
	{
		head_chain = head_group;
		while (head_chain != NULL)
		{
			dump_word(head_group, head_chain);
			head_chain = head_chain->next_chain;
		}
		head_group = head_group->next_group;
	}
}

static void	dump_argv(char **argv)
{
	size_t	index;

	index = 0;
	while (argv[index] != NULL)
	{
		print_fd(STDOUT_FILENO, "%u: %s\n", index, argv[index]);
		index++;
	}
}

static t_result	handle_line(char *line, t_state *state)
{
	t_word			*root_word;
	t_result		result;
	t_cmd_builder	builder;

	result = word_chain_from_string(&root_word, slice0(line));
	if (result != S_OK)
	{
		free(line);
		return (result);
	}
	add_history(line);
	result = insert_vars(root_word, state);
	if (result == S_OK)
	{
		dump_words(root_word);
		result = cmd_builder_build_argv(&builder, root_word);
		if (result == S_OK)
		{
			dump_argv(builder.argv);
			ms_ptrs_free(builder.argv);
		}
		words_clr(&root_word);
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
