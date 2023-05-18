/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:58:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 15:30:59 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

// TODO: Type for context pointer
typedef int	(*t_builtin_fn)(int argc, char *argv[], int out_fd, void *context);

typedef struct s_builtin
{
	const char		*name;
	t_builtin_fn	main_fn;
}	t_builtin;

int	builtin_pwd(int argc, char *argv[], int out_fd, void *context);

#endif // !BUILTIN_H