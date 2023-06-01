/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:14:52 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/01 16:38:06 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../util/util.h"
#include "../state/state.h"

int	print_exports(t_var *var, int out_fd)
{
	t_var	*root_var;
	t_var	*next_print_var;

	root_var = var;
	if (var == NULL)
		return (0);
	next_print_var = var;
	while (var != NULL)
	{
		if (var->flags & VAR_EXPORT)
			print_fd(out_fd, "declare -x %s=\"%s\"\n", var->name, var->value);
		var = var->next;
	}
	return (0);
}

int	export_vars(char *argv[], t_state *state)
{
	t_slice	name;
	t_slice	value;
	t_var	*var;

	split_once(slice0(*argv), begin_delimiter, &name, &value);
	value = advance(value);
	var = vars_set(&state->root_var, name, value);
	if (var == NULL)
		return (1);
	var->flags |= VAR_EXPORT;
	return (0);
}

int	builtin_export(int argc, char *argv[], int out_fd, t_state *state)
{
	int		error_detected;

	(void) argc;
	argv++;
	error_detected = 0;
	if (argv[0] == NULL)
		return (print_exports(state->root_var, out_fd));
	while (*argv != NULL)
	{
		if (!((ms_isalpha(**argv)) || **argv == '_')
			|| ms_strchr(*argv, '=') == NULL)
		{
			print_fd(out_fd,
				"minishell: export: `%s': not a valid identifier\n", *argv);
			error_detected = 1;
		}
		else
			error_detected = export_vars(argv, state);
		argv++;
	}
	return (error_detected);
}
