/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 10:33:06 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/23 00:35:05 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../util/util.h"
#include "../state/state.h"
#include "../parser/parser.h"

#include <unistd.h>

static void	print_value_with_backslash(int out_fd, char *var_value)
{
	while (*var_value != '\0')
	{
		if (*var_value == '"' || *var_value == '$')
		{
			print_fd(out_fd, "\\%c", *var_value);
		}
		else
			print_fd(out_fd, "%c", *var_value);
		var_value++;
	}
	print_fd(out_fd, "\"\n");
}

static int	print_exports(t_var *var, int out_fd)
{
	if (var == NULL)
		return (0);
	while (var != NULL)
	{
		if (var->flags & VAR_EXPORT)
		{
			if (*var->value != '\0'
				|| var->flags & var->flags & VAR_EXPLICIT_EMPTY)
			{
				print_fd(out_fd, "declare -x %s=\"", var->name);
				print_value_with_backslash(out_fd, var->value);
			}
			else
				print_fd(out_fd, "declare -x %s\n", var->name);
		}
		var = var->next;
	}
	return (0);
}

static int	set_export(t_state *state, const char *argument)
{
	t_var	*var;
	t_slice	name;
	t_slice	value;
	t_slice	remainder;
	int		has_equal_sign;

	split_once(slice0(argument), begin_delimiter, &name, &value);
	has_equal_sign = consume(&value, "=");
	split_once(name, begin_not_identifier, &name, &remainder);
	if (name.size == 0 || remainder.size > 0)
	{
		print_fd(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", argument);
		return (0);
	}
	var = vars_get(&state->root_var, name);
	if (var == NULL || has_equal_sign)
		var = vars_set(&state->root_var, name, value);
	if (var == NULL)
		return (1);
	var->flags |= VAR_EXPORT;
	if (value.size == 0 && has_equal_sign)
		var->flags |= VAR_EXPLICIT_EMPTY;
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
