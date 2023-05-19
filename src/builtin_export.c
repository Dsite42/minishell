/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 16:14:52 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/19 12:13:34 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "util.h"

// the ft_splits are just used to imitate the behavior of already getting the correct parsed arguments
// TEST=10 abc: abc should be ignored
// TEST==100: should export "TEST==100"
// =100: minishell: export: `=100': not a valid identifier
#include "libft.h"

int	builtin_export(int argc, char *argv[], int out_fd, void *context)
{
	char	**splitted;
	t_var	*var;

	if (!((*(argv[2]) >= 'a' && *(argv[2]) <= 'z')
			|| (*(argv[2]) >= 'A' && *(argv[2]) <= 'Z')))
	{
		print_fd(out_fd,
			"minishell: export: `%s': not a valid identifier\n", argv[2]);
		return (1);
	}
	splitted = ft_split(argv[2], '=');
	if (splitted == NULL)
		return (1);
	var = vars_set(&((t_storage *)context)->root_var,
			slice0(splitted[0]),
			slice0(ft_split(ft_strchr(argv[2], '=') + 1, ' ')[0]));
	if (var == NULL)
		return (1);
	var->flags |= VAR_EXPORT;
}
