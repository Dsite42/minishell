/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:11:15 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 22:12:49 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_H
# define PRIVATE_H

# include "parser.h"

typedef struct s_word_builder
{
	t_word	*root_word;
	t_word	*group_head_word;
	t_word	*chain_head_word;
	int		is_new_group;
}	t_word_builder;

typedef struct s_cmd_builder
{
	size_t	size;
	size_t	index;
	char	*iter;
	char	**argv;
}	t_cmd_builder;

// word_builder.c
void		word_builder_group(t_word_builder *self);
t_result	word_builder_append(t_word_builder *self,
				unsigned int flags, t_slice slice, int ignore_empty);
t_result	word_builder_clean_return(t_word_builder *self, t_result result);

#endif // !PRIVATE_H