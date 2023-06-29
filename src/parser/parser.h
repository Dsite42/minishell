/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:55:33 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/29 18:34:58 by jsprenge         ###   ########.fr       */
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
# define WORD_VAR_PID 65

// TODO: (Maybe) Add third "next_command" link

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

typedef struct s_redir
{
	struct s_redir	*next;
	int				type;
	char			name[];
}	t_redir;

typedef struct s_cmd
{
	struct s_cmd	*next;
	t_redir			*root_redir;
	char			*argv[];
}	t_cmd;

// insert_vars.c
t_result	expand_vars(t_word *head_group, t_state *state);

// grammar.c
size_t		begin_space(t_slice slice);
size_t		begin_not_identifier(t_slice slice);
size_t		begin_word_split(t_slice slice);
size_t		begin_single_quote_split(t_slice slice);
size_t		begin_double_quote_split(t_slice slice);

// iter_cmd_args.c
t_result	words_iter_cmd_args(void *context_ptr, t_word **p_head_group,
				t_result (*callback)(void *context_ptr,
					int is_group, t_word *word));

// words.c
int			word_new(t_word **p_new_word, unsigned int flags, t_slice slice);
char		**argv_from_word_group(t_word *root_word, t_var **p_root_var);
void		words_clr(t_word **p_root_word);

// parser.c
t_result	word_chain_from_string(t_word **p_root_word, t_slice remainder);

#endif // !PARSER_H