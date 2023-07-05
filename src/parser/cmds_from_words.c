/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_from_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 21:39:05 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 00:47:59 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

static t_result	drop_with_result(
	t_cmd_builder *builder, t_cmd **p_root_cmd, t_result result)
{
	t_redir	*head_redir;
	t_redir	*next_redir;

	if (builder->argv != NULL)
		ms_ptrs_free(builder->argv);
	head_redir = builder->root_redir;
	while (head_redir != NULL)
	{
		next_redir = head_redir->next;
		free(head_redir);
		head_redir = next_redir;
	}
	cmd_clear(p_root_cmd);
	return (result);
}

static void	populate_and_append(t_cmd_builder *builder,
	t_cmd **p_root_cmd, t_cmd **p_head_cmd, t_cmd *new_cmd)
{
	new_cmd->argv = builder->argv;
	new_cmd->root_redir = builder->root_redir;
	new_cmd->next = NULL;
	if (*p_root_cmd == NULL)
	{
		*p_root_cmd = new_cmd;
		*p_head_cmd = new_cmd;
		return ;
	}
	(*p_head_cmd)->next = new_cmd;
	*p_head_cmd = new_cmd;
}

static t_result	build_cmd(
		t_word *root_word, t_cmd **p_root_cmd, t_cmd **p_head_cmd)
{
	t_result		result;
	t_cmd_builder	builder;
	t_cmd			*new_cmd;

	ms_zero(&builder, sizeof(builder));
	result = cmd_builder_build_argv(&builder, root_word);
	if (result != S_OK)
		return (drop_with_result(&builder, p_root_cmd, result));
	result = cmd_builder_build_redirs(&builder, root_word);
	if (result != S_OK)
		return (drop_with_result(&builder, p_root_cmd, result));
	new_cmd = malloc(sizeof(t_cmd));
	if (new_cmd == NULL)
		return (drop_with_result(&builder, p_root_cmd, result));
	populate_and_append(&builder, p_root_cmd, p_head_cmd, new_cmd);
	return (S_OK);
}

t_result	cmds_from_words(
		t_word *root_word, t_cmd **p_root_cmd, t_state *state)
{
	t_result	result;
	t_cmd		*head_cmd;
	t_word		*head_word;

	insert_vars_pre(root_word);
	result = insert_vars(root_word, state);
	if (result != S_OK)
		return (result);
	result = check_syntax(root_word, state);
	if (result != S_OK)
		return (result);
	*p_root_cmd = NULL;
	head_word = root_word;
	while (head_word != NULL)
	{
		result = build_cmd(head_word, p_root_cmd, &head_cmd);
		if (result != S_OK)
			return (result);
		while (head_word != NULL
			&& (head_word->flags & WORD_OP_MASK) != WORD_OP_PIPE)
			head_word = head_word->next_group;
		if (head_word != NULL)
			head_word = head_word->next_group;
	}
	return (S_OK);
}
