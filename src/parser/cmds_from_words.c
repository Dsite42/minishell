/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_from_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 21:39:05 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/01 20:14:10 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

static t_result	build_cmd(
		t_word *root_word, t_cmd **p_root_cmd, t_cmd **p_head_cmd)
{
	t_result		result;
	t_cmd_builder	builder;
	t_cmd			*new_cmd;

	ms_zero(&builder, sizeof(builder));
	result = cmd_builder_build_argv(&builder, root_word);
	if (result != S_OK)
		return (result);
	result = cmd_builder_build_redirs(&builder, root_word);
	if (result != S_OK)
	{
		// TODO: Free partial command in builder
		return (result);
	}
	new_cmd = malloc(sizeof(t_cmd));
	if (new_cmd == NULL)
	{
		// TODO: Free full command in builder
		return (E_NOMEM);
	}
	new_cmd->argv = builder.argv;
	new_cmd->root_redir = builder.root_redir;
	new_cmd->next = NULL;
	if (*p_root_cmd == NULL)
	{
		*p_root_cmd = new_cmd;
		*p_head_cmd = new_cmd;
		return (S_OK);
	}
	(*p_head_cmd)->next = new_cmd;
	*p_head_cmd = new_cmd;
	return (S_OK);
}

t_result	cmds_from_words(
		t_word *root_word, t_cmd **p_root_cmd, t_state *state)
{
	t_result	result;
	t_cmd		*head_cmd;
	t_word		*head_word;

	result = insert_vars(root_word, state);
	if (result != S_OK)
		return (result);
	result = check_syntax(root_word);
	if (result != S_OK)
		return (result);
	*p_root_cmd = NULL;
	head_word = root_word;
	while (head_word != NULL)
	{
		if ((head_word->flags & WORD_OP_MASK) == WORD_OP_PIPE)
		{
			head_word = head_word->next_group;
			if (head_word == NULL)
				break ;
		}
		result = build_cmd(head_word, p_root_cmd, &head_cmd);
		if (result != S_OK)
			return (result);
		while (head_word != NULL && (head_word->flags & WORD_OP_MASK) != WORD_OP_PIPE)
		{
			head_word = head_word->next_group;
		}
	}
	return (S_OK);
}
