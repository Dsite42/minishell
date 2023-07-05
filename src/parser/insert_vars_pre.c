/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_vars_pre.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 00:19:48 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 00:40:09 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

// Transforms WORD_VARs following a heredoc operator into WORD_LITERAL_VARs
void	insert_vars_pre(t_word *head_group)
{
	t_word	*head_chain;

	while (head_group != NULL)
	{
		if ((head_group->flags & WORD_OP_MASK) == WORD_OP_HEREDOC
			&& head_group->next_group != NULL && (head_group->next_group->flags
				& WORD_OP_MASK) != WORD_OP_PIPE)
		{
			head_group = head_group->next_group;
			head_chain = head_group;
			while (head_chain != NULL)
			{
				if (head_chain->flags & WORD_VAR)
					head_chain->flags = (head_chain->flags & ~WORD_VAR)
						| WORD_LITERAL_VAR;
				head_chain = head_chain->next_chain;
			}
		}
		head_group = head_group->next_group;
	}
}
