/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:55:33 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 18:49:58 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../util/util.h"
# include "../state/state.h"

// Words are organized into groups of chains, eg. the following command ...
//    echo -n "Hello, $NAME, nice to meet you."
//
// ... is parsed into the following structure of words:
//    ["echo"] -> next_group -> ["-n"] -> next_group -> [      "Hello, "      ]
//                                                                 |
//                                                            next_chain
//                                                                 |
//                                                      [       "NAME"        ]
//                                                                 |
//                                                            next_chain
//                                                                 |
//                                                      [", nice to meet you."]

# define WORD_VAR 1

typedef struct s_word
{
	struct s_word	*next_group;
	struct s_word	*next_chain;
	unsigned int	flags;
	t_slice			slice;
	void			*cache;
}	t_word;

// NOTE: t_word.next_group is only valid for the first node of a chain
// NOTE: The validity of t_word.cache is up to functions writing to it

// words.c
int			word_new(t_word **p_new_word, unsigned int flags, t_slice slice);
char		**argv_from_word_group(t_word *root_word, t_var **p_root_var);
int			words_clr(t_word **p_root_word);

// parser.c
t_result	word_chain_from_string(t_word **p_root_word, t_slice remainder);

#endif // !PARSER_H