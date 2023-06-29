/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:28:41 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 22:16:11 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <stdlib.h>

int	word_new(t_word **p_new_word, unsigned int flags, t_slice slice)
{
	t_word	*new_word;

	new_word = malloc(sizeof(t_word));
	if (new_word == NULL)
		return (0);
	new_word->flags = flags;
	new_word->slice = slice;
	new_word->next_group = NULL;
	new_word->next_chain = NULL;
	*p_new_word = new_word;
	return (1);
}

void	words_clr(t_word **p_root_word)
{
	t_word	*group_head_word;
	t_word	*group_next_word;
	t_word	*chain_head_word;
	t_word	*chain_next_word;

	group_head_word = *p_root_word;
	while (group_head_word != NULL)
	{
		chain_head_word = group_head_word;
		group_next_word = group_head_word->next_group;
		while (chain_head_word != NULL)
		{
			chain_next_word = chain_head_word->next_chain;
			free(chain_head_word);
			chain_head_word = chain_next_word;
		}
		group_head_word = group_next_word;
	}
	*p_root_word = NULL;
}
