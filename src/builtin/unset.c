/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 14:10:59 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/03 10:38:43 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../state/state.h"

int	builtin_unset(int argc, char *argv[], int out_fd, t_state *state)
{
	int		error_detected;

	(void)argc;
	argv++;
	while (*argv != NULL)
	{
		if (!((ms_isalpha(**argv)) || **argv == '_')
			|| ms_strchr(*argv, '=') != NULL)
		{
			print_fd(out_fd,
				"minishell: unset: `%s': not a valid identifier\n",
				*argv);
			error_detected = 1;
		}
		vars_del(&state->root_var, slice0(*argv));
		argv++;
	}
	return (error_detected);
}
