/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 10:33:06 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/04 22:05:00 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../util/util.h"
#include "../state/state.h"
#include "../parser/parser.h"

#include <unistd.h>

static int	print_exports(t_var *var, int out_fd)
{
	if (var == NULL)
		return (0);
	while (var != NULL)
	{
		if (var->flags & VAR_EXPORT)
			print_fd(out_fd, "declare -x %s=\"%s\"\n", var->name, var->value);
		var = var->next;
	}
	return (0);
}

// TODO: Edge-cases with empty values, etc, etc
static int	set_export(t_state *state, const char *argument)
{
	t_var	*var;
	t_slice	name;
	t_slice	value;
	t_slice	remainder;

	split_once(slice0(argument), begin_delimiter, &name, &value);
	value = advance(value);
	split_once(name, begin_not_identifier, &name, &remainder);
	if (name.size == 0 || remainder.size > 0)
	{
		print_fd(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", argument);
		return (0);
	}
	var = vars_set(&state->root_var, name, value);
	if (var == NULL)
		return (0);
	var->flags |= VAR_EXPORT;
	return (1);
}

int	builtin_export(int argc, char *argv[], int out_fd, t_state *state)
{
	int	index;
	int	return_code;

	(void) out_fd;
	if (argc == 1)
		return (print_exports(state->root_var, out_fd));
	index = 1;
	return_code = 0;
	while (index < argc)
	{
		if (!set_export(state, argv[index]))
			return_code = 1;
		index++;
	}
	return (return_code);
}
