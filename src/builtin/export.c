/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:14:52 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/20 15:32:20 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "util.h"
#include <limits.h>

// the ft_splits are just used to imitate the behavior of already getting the correct parsed arguments
// TEST=10 abc: abc should be ignored --> done
// TEST==100: should export "TEST==100" --> done
// =100: minishell: export: `=100': not a valid identifier --> done
// TEST1==10 TEST2==100: should export both --> done
// export: print all exported variables in alphabetical order --> done
#include "../libft.h"

void	of_free_arr(void **arr, int self_free)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	if (self_free == 1 && arr != NULL)
		free(arr);
}

void	print_exports(t_var *var)
{
	t_var	*root_var;
	t_var	*next_print_var;

	root_var = var;
	if (var == NULL)
		return ;
	next_print_var = var;
	while (var != NULL)
	{
		if (var->flags & VAR_EXPORT)
			printf("declare -x %s=\"%s\"\n", var->name, var->value);
		var = var->next;
	}
}

int	builtin_export(int argc, char *argv[], int out_fd, t_state *state)
{
	char	**splitted;
	t_var	*var;
	int		error_detected;
	int		i;

	(void) argc;
	argv = ft_split(argv[2], ' ');
	i = 0;
	error_detected = 0;
	if (argv[0] == NULL)
	{
		print_exports(state->root_var);
		return (error_detected);
	}
	while (argv[i] != NULL)
	{
		if (!((ms_isalpha(*(argv[i]))) || *(argv[i]) == '_')
			|| ms_strchr(argv[i], '=') == NULL)
		{
			print_fd(out_fd,
				"minishell: export: `%s': not a valid identifier\n", argv[i]);
			error_detected = 1;
		}
		else
		{
			splitted = ft_split(argv[i], '=');
			if (splitted == NULL)
				error_detected = 1;
			var = vars_set(&state->root_var,
					slice0(splitted[0]),
					slice0(ft_strchr(argv[i], '=') + 1));
			if (var == NULL)
				error_detected = 1;
			var->flags |= VAR_EXPORT;
			of_free_arr((void **)splitted, 1);
		}
		i++;
	}
	of_free_arr((void **)argv, 1);
	return (error_detected);
}
