/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:17:03 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/20 17:05:34 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

# include "../util/util.h"

# define VAR_EXPORT 1
# define VAR_EXPLICIT_EMPTY 2

typedef struct s_var
{
	struct s_var	*next;
	unsigned int	flags;
	char			*value;
	char			name[];
}	t_var;

typedef struct s_state
{
	t_var	*root_var;
	int 	exit_code;
}	t_state;

// vars.c
t_var	*vars_get(t_var **p_root_var, t_slice name);
void	vars_del(t_var **p_root_var, t_slice name);
void	vars_clr(t_var **p_root_var);
t_var	*var_new(t_slice name, t_slice value);

// vars_set.c
t_var	*vars_set(t_var **p_root_var, t_slice name, t_slice value);

// env.c
int		vars_from_envp(char *envp[], t_var **p_root_var);
char	**envp_from_vars(t_var **p_root_var);
int		is_delimiter(char input);
size_t	begin_delimiter(t_slice slice);

#endif // !STATE_H