/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:58:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/20 16:03:36 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../state/state.h"

typedef int	(*t_builtin_fn)(int argc, char *argv[], int out_fd, t_state *state);

typedef struct s_builtin
{
	const char		*name;
	t_builtin_fn	main_fn;
}	t_builtin;

int	builtin_pwd(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_cd(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_env(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_exit(int argc, char *argv[], int out_fd, t_state *state);

#endif // !BUILTIN_H