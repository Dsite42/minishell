/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 14:10:59 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/20 15:04:07 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../state/state.h"

// ft_split is used to imitate the parser
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

int	builtin_unset(int argc, char *argv[], int out_fd, t_state *state)
{
	char	**splitted_argvs;
	char	**splitted_argvs_root;
	int		error_detected;

	(void)out_fd;
	splitted_argvs = ft_split(argv[2], ' ');
	splitted_argvs_root = splitted_argvs;
	while (*splitted_argvs != NULL)
	{
		if (!((ms_isalpha(**splitted_argvs)) || **splitted_argvs == '_')
			|| ms_strchr(*splitted_argvs, '=') != NULL)
		{
			print_fd(out_fd,
				"minishell: unset: `%s': not a valid identifier\n",
				*splitted_argvs);
			error_detected = 1;
		}
		vars_del(&state->root_var, slice0(*splitted_argvs));
		splitted_argvs++;
	}
	of_free_arr((void **)splitted_argvs_root, 1);
	return (error_detected);
}
