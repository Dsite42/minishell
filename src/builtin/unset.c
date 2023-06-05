/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 14:10:59 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/04 21:54:31 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../state/state.h"
#include "../parser/parser.h"

#include <unistd.h>

int	builtin_unset(int argc, char *argv[], int out_fd, t_state *state)
{
	t_slice	name;
	int		index;
	t_slice	argument;
	int		return_code;

	(void) out_fd;
	index = 1;
	return_code = 0;
	while (index < argc)
	{
		split_once(slice0(argv[index]), begin_not_identifier, &name, &argument);
		if (name.size == 0 || argument.size > 0)
		{
			print_fd(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n",
				argv[index]);
			return_code = 1;
		}
		else
		{
			vars_del(&state->root_var, name);
		}
		index++;
	}
	return (return_code);
}
