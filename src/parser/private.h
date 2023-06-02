/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:11:15 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 23:28:36 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_H
# define PRIVATE_H

# include "parser.h"

typedef struct s_builder
{
	t_word	*root_word;
	t_word	*group_head_word;
	t_word	*chain_head_word;
	int		is_new_group;
}	t_builder;

// builder.c
void		builder_group(t_builder *self);
t_result	builder_append(t_builder *self, unsigned int flags, t_slice slice);
t_result	builder_clean_return(t_builder *self, t_result result);

// grammar.c
size_t		begin_space(t_slice slice);
size_t		begin_not_identifier(t_slice slice);
size_t		begin_word_split(t_slice slice);
size_t		begin_single_quote_split(t_slice slice);
size_t		begin_double_quote_split(t_slice slice);

#endif // !PRIVATE_H