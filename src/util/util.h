/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 15:59:24 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 13:38:51 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

// Every function returning an int as a return code is expected to be boolean,
// zero for failure, one for success
// If a function wants to use the codes below, it MUST return t_result instead

# define S_OK 0
# define E_BUG -1
# define E_NOMEM -2
# define E_SQTERM -3
# define E_DQTERM -4
# define E_SYNPIPE -5
# define E_SYNLF -6
# define E_SYNAMB -7
# define E_SYNREAD -8
# define E_SYNWRITE -9
# define E_SYNAPPEND -10
# define E_SYNHEREDOC -11

// Flags for terminal and signal behavior

# define TTY_IS_CHILD 0
# define TTY_HEREDOC 1

typedef struct s_slice
{
	const char	*data;
	size_t		size;
}	t_slice;

typedef struct s_strbuf
{
	char	*data;
	size_t	length;
	size_t	capacity;
}	t_strbuf;

typedef int	t_result;

// libms_part0.c
size_t		ms_ptrs_count(void *ptrs);
void		*ms_ptrs_free(void *ptrs);
int			ms_is_digit(int input);
int			ms_is_alpha(int input);
size_t		ms_str_length(const char *string);

// libms_part1.c
char		*ms_str_find(const char *string, int predicate);
int			ms_str_compare(
				const char *string1, const char *string2, size_t limit);
void		*ms_copy(void *destination, const void *source, size_t size);
void		ms_zero(void *destination, size_t size);
void		*ms_zero_alloc(size_t size, size_t count);

// libms_part2.c
char		*ms_join_slices(size_t count, ...);
char		*ms_mem_clone(const void *data, size_t size);
char		*ms_int_to_str(int value);
const char	*ms_result_to_string(t_result result);

// print_core.c
int			print_fd(int fd, const char *format, ...);

// slice_core.c
t_slice		advance(t_slice slice);
size_t		split_once(t_slice slice, size_t (*predicate)(t_slice),
				t_slice *p_part0, t_slice *p_part1);
void		split_at(t_slice slice, size_t index,
				t_slice *p_part0, t_slice *p_part1);
t_slice		trim_left(t_slice slice, size_t (*predicate)(t_slice),
				size_t *p_count);
int			consume(t_slice *p_slice, const char *predicate);

// slice_parse.c
int			parse_int(int *p_result, t_slice *p_slice);

// slice_str.c
t_slice		slice0(const char *string);
char		*slice_to_str(t_slice slice);
int			slice_str_equal(t_slice slice, const char *string);
size_t		slice_str_begin(t_slice slice, const char *string);

// strbuf.c
int			strbuf_init(t_strbuf *self);
int			strbuf_push(t_strbuf *self, t_slice slice);
void		strbuf_drop(t_strbuf *self);
void		strbuf_take(t_strbuf *self, char **p_string);

// tty.c
void		tty_setup(void);
void		tty_enter(int is_child);
int			tty_get_flag(unsigned int index);
void		tty_set_flag(unsigned int index, int enable);

#endif // !UTIL_H