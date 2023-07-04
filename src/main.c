/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 18:42:43 by jsprenge         ###   ########.fr       */
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

#define FLAGS "VO---QSI"

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
		if (head_chain->flags & WORD_SOURCE)
			print_fd(STDOUT_FILENO, "  [%s] '%a' (%a)\n", flag_prefix,
				head_chain->slice, head_chain->source);
		else
			print_fd(STDOUT_FILENO, "  [%s] '%a'\n", flag_prefix,
				head_chain->slice);
		return ;
	}
	if (head_chain == head_group)
		join_prefix = "╭ ";
	else if (head_chain->next_chain == NULL)
		join_prefix = "╰ ";
	else
		join_prefix = "│ ";
	if (head_chain->flags & WORD_SOURCE)
		print_fd(STDOUT_FILENO, "%s[%s] '%a' (%a)\n",
			join_prefix, flag_prefix, head_chain->slice, head_chain->source);
	else
		print_fd(STDOUT_FILENO, "%s[%s] '%a'\n",
			join_prefix, flag_prefix, head_chain->slice);
}

void	dump_words(t_word *head_group)
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

void	dump_cmds(t_cmd *head_cmd)
{
	size_t	count;
	size_t	index;
	t_redir	*head_redir;

	count = 0;
	while (head_cmd != NULL)
	{
		print_fd(STDOUT_FILENO, "  Command #%u\n", count++);
		print_fd(STDOUT_FILENO, "    argv: [");
		index = 0;
		while (head_cmd->argv[index] != NULL)
		{
			print_fd(STDOUT_FILENO, "'%s'", head_cmd->argv[index]);
			if (head_cmd->argv[index + 1] != NULL)
				print_fd(STDOUT_FILENO, ", ");
			index++;
		}
		print_fd(STDOUT_FILENO, "]\n");
		print_fd(STDOUT_FILENO, "    redirs: [");
		head_redir = head_cmd->root_redir;
		while (head_redir != NULL)
		{
			print_fd(STDOUT_FILENO, "{ %i, '%s' }", head_redir->type, head_redir->name);
			if (head_redir->next != NULL)
				print_fd(STDOUT_FILENO, ", ");
			head_redir = head_redir->next;
		}
		print_fd(STDOUT_FILENO, "]\n");
		head_cmd = head_cmd->next;
	}
}

static t_result	handle_line(char *line, t_state *state, char **envp)
{
	t_result	result;
	t_cmd		*root_cmd;
	t_word		*root_word;

	add_history(line);
	result = words_from_slice(&root_word, slice0(line));
	if (result == S_OK)
	{
		//print_fd(STDOUT_FILENO, "Word tree before cmds_from_words():\n");
		//dump_words(root_word);
		result = cmds_from_words(root_word, &root_cmd, state);
		if (result == S_OK)
		{
			//print_fd(STDOUT_FILENO, "Word tree after cmds_from_words():\n");
			//dump_words(root_word);
			//print_fd(STDOUT_FILENO, "Result of cmds_from_words():\n");
			//dump_cmds(root_cmd);
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
	t_state				state;
	t_result			result;
	char				*line;

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
		result = handle_line(line, &state, envp);
		if (result != S_OK)
			print_fd(STDERR_FILENO, "Error code %i while parsing\n", result);
	}
	write(STDERR_FILENO, "exit\n", 5);
	state_drop(&state);
	return (0);
}