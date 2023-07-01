/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 17:49:16 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/01 20:33:49 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

// TODO: Implement E_SYNAMB and E_SYNLF

t_result	check_syntax(t_word *head_group)
{
	t_word	*prev_group;

	prev_group = NULL;
	while (head_group != NULL)
	{
		if (prev_group != NULL && prev_group->flags & WORD_IS_OP
			&& head_group->flags & WORD_IS_OP)
		{
			if ((prev_group->flags & WORD_OP_MASK) == WORD_OP_PIPE)
				return (E_SYNPIPE);
			if ((head_group->flags & WORD_OP_MASK) == WORD_OP_PIPE)
				return (E_SYNPIPE);
			if ((head_group->flags & WORD_OP_MASK) == WORD_OP_READ)
				return (E_SYNREAD);
			if ((head_group->flags & WORD_OP_MASK) == WORD_OP_WRITE)
				return (E_SYNWRITE);
			if ((head_group->flags & WORD_OP_MASK) == WORD_OP_APPEND)
				return (E_SYNAPPEND);
			if ((head_group->flags & WORD_OP_MASK) == WORD_OP_HEREDOC)
				return (E_SYNHEREDOC);
		}
		prev_group = head_group;
		head_group = head_group->next_group;
	}
	return (S_OK);
}
