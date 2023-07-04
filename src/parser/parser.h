/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:55:33 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 00:36:22 by jsprenge         ###   ########.fr       */
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
# define WORD_IS_OP 2
# define WORD_OP_PIPE 2
# define WORD_OP_READ 6
# define WORD_OP_WRITE 10
# define WORD_OP_APPEND 14
# define WORD_OP_HEREDOC 18
# define WORD_OP_MASK 30
# define WORD_QUOTE 32
# define WORD_SOURCE 64
# define WORD_INSERT 128
# define WORD_LITERAL_VAR 256

typedef struct s_word
{
	struct s_word	*next_group;
	struct s_word	*next_chain;
	unsigned int	flags;
	t_slice			slice;
	t_slice			source;
}	t_word;

// NOTE: t_word.next_group is only valid for the first node of a chain

typedef struct s_redir
{
	struct s_redir	*next;
	unsigned int	type;
	char			name[];
}	t_redir;

typedef struct s_cmd
{
	struct s_cmd	*next;
	t_redir			*root_redir;
	char			**argv;
}	t_cmd;

// cmds_from_words.c
t_result	cmds_from_words(t_word *root_word, t_cmd **p_root_cmd,
				t_state *state);

// grammar.c
size_t		begin_space(t_slice slice);
size_t		begin_not_identifier(t_slice slice);
size_t		begin_word_split(t_slice slice);
size_t		begin_single_quote_split(t_slice slice);
size_t		begin_double_quote_split(t_slice slice);

// parser_core.c
void		word_clear(t_word **p_root_word);
void		cmd_clear(t_cmd **p_root_cmd);

// words_from_slice.c
t_result	words_from_slice(t_word **p_root_word, t_slice slice);

#endif // !PARSER_H
