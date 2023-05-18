/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:17:03 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/18 16:37:17 by jsprenge         ###   ########.fr       */
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
}	t_state;

// vars.c
t_var	*vars_get(t_var **p_root_var, t_slice name);
t_var	*vars_set(t_var **p_root_var, t_slice name, t_slice value);
void	vars_del(t_var **p_root_var, t_slice name);
void	vars_clr(t_var **p_root_var);

// env.c
int		vars_from_envp(char *envp[], t_var **p_root_var);
char	**envp_from_vars(t_var **p_root_var);

#endif // !STATE_H