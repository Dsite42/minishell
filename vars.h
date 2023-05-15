/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:17:03 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/15 18:22:27 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARS_H
# define VARS_H

# include "slice.h"

typedef struct s_var
{
	struct s_var	*next;
	unsigned int	flags;
	char			*value;
	char			name[];
}	t_var;

t_var	*vars_get(t_var **p_root_var, t_slice name);
t_var	*vars_set(t_var **p_root_var, t_slice name, t_slice value);
void	vars_del(t_var **p_root_var, t_slice name);
void	vars_clr(t_var **p_root_var);

#endif // !VARS_H