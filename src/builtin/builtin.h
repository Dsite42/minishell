/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:58:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/04 21:22:08 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../state/state.h"

typedef int	(*t_builtin_fn)(int argc, char *argv[], int out_fd, t_state *state);

int	builtin_pwd(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_cd(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_env(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_unset(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_echo(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_exit(int argc, char *argv[], int out_fd, t_state *state);
int	builtin_export(int argc, char *argv[], int out_fd, t_state *state);

#endif // !BUILTIN_H