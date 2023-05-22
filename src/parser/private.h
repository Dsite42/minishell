/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:11:15 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 15:31:18 by jsprenge         ###   ########.fr       */
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
void	builder_group(t_builder *builder);
int		builder_append(t_builder *builder, unsigned int flags, t_slice slice);

// grammar.c
int		is_space(char input);
int		is_not_identifier(char input);
int		is_word_split(char input);
int		is_single_quote_split(char input);
int		is_double_quote_split(char input);

#endif // !PRIVATE_H