/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:28:41 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/20 00:47:24 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include <stdlib.h>

static char	*word_chain_copy(t_word *head_word, char *new_string)
{
	t_var	*var;
	char	*iter;
	t_slice	slice;

	if (new_string == NULL)
		return (NULL);
	iter = new_string;
	while (head_word != NULL)
	{
		if (head_word->flags & WORD_VAR)
		{
			var = head_word->cache;
			if (var != NULL)
				slice = slice0(var->value);
			else
				slice.size = 0;
		}
		else
			slice = head_word->slice;
		ms_memcpy(iter, slice.data, slice.size);
		iter += slice.size;
		head_word = head_word->next_chain;
	}
	*iter = '\0';
	return (new_string);
}

static int	word_chain_to_arg(
		t_word *root_word, char **p_arg, t_var **p_root_var)
{
	t_var	*var;
	size_t	length;
	t_word	*head_word;

	length = 0;
	head_word = root_word;
	while (head_word != NULL)
	{
		if (head_word->flags & WORD_VAR)
		{
			var = vars_get(p_root_var, head_word->slice);
			if (var != NULL)
				length += ms_strlen(var->value);
			head_word->cache = var;
		}
		else
			length += head_word->slice.size;
		head_word = head_word->next_chain;
	}
	*p_arg = word_chain_copy(root_word, malloc(length + 1));
	if (*p_arg == NULL)
		return (0);
	return (1);
}

char	**word_group_to_argv(t_word *root_word, t_var **p_root_var)
{
	size_t	index;
	size_t	count;
	t_word	*head_word;
	char	**new_argv;

	count = 0;
	head_word = root_word;
	while (head_word != NULL)
	{
		count++;
		head_word = head_word->next_group;
	}
	new_argv = ms_calloc(sizeof(char *), count + 1);
	if (new_argv == NULL)
		return (NULL);
	index = 0;
	head_word = root_word;
	while (index < count)
	{
		if (!word_chain_to_arg(head_word, &new_argv[index++], p_root_var))
			return (free_pointers(new_argv));
		head_word = head_word->next_group;
	}
	return (new_argv);
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
}
