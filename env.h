/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 18:28:51 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 12:28:10 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "vars.h"

int		vars_from_envp(char *envp[], t_var **p_root_var);
char	**envp_from_vars(t_var **p_root_var);

#endif