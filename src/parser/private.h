/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:11:15 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 00:48:17 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATE_H
# define PRIVATE_H

# include "parser.h"
# include "../state/state.h"

typedef struct s_word_builder
{
	t_word	*root_word;
	t_word	*group_head_word;
	t_word	*chain_head_word;
	t_word	*ensure_head_word;
	int		is_new_group;
}	t_word_builder;

typedef struct s_cmd_builder
{
	size_t	size;
	size_t	index;
	char	*iter;
	char	**argv;
	t_redir	*root_redir;
}	t_cmd_builder;

// check_syntax.c
t_result	check_syntax(t_word *head_group, t_state *state);

// cmd_builder_argv.c
t_result	cmd_builder_build_argv(t_cmd_builder *self, t_word *root_word);

// cmd_builder_redirs.c
t_result	cmd_builder_build_redirs(t_cmd_builder *self, t_word *root_word);

// insert_vars_pre.c
void		insert_vars_pre(t_word *head_group);

// insert_vars.c
t_result	insert_vars(t_word *head_group, t_state *state);

// parser_core.c
int			word_new(t_word **p_new_word, unsigned int flags, t_slice slice);

// word_builder.c
t_result	word_builder_ensure_one(t_word_builder *self,
				int is_end, unsigned int flags);
t_result	word_builder_append(t_word_builder *self,
				unsigned int flags, t_slice slice, int ignore_empty);
t_result	word_builder_clean_return(t_word_builder *self, t_result result);

#endif // !PRIVATE_H