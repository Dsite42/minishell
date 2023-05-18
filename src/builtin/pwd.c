/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 16:17:41 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/18 16:37:46 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "../util/util.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>

int	builtin_pwd(int argc, char *argv[], int out_fd, t_state *state)
{
	int		index;
	char	buffer[MAXPATHLEN + 1];

	(void) state;
	index = 1;
	while (index < argc)
	{
		if (argv[index][0] == '-')
		{
			print_fd(STDERR_FILENO, "minishell: pwd: %s: invalid option\n",
				argv[index]);
			return (1);
		}
		index++;
	}
	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("pwd: getcwd() failed");
		return (1);
	}
	print_fd(out_fd, "%s\n", buffer);
	return (0);
}
