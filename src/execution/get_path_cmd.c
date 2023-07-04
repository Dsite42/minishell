/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 12:23:59 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/04 18:25:12 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static size_t	begin_delimiter2(t_slice slice)
{
	return (slice.size > 0 && *slice.data == ':');
}

static char	*join_path_cmd(t_slice splitted_path, char *argv)
{
	char	*path_cmd;
	char	*path_cmd_start;

	path_cmd = (char *)malloc((splitted_path.size
				+ 1 + ms_str_length(argv) + 1) * sizeof(char));
	path_cmd_start = path_cmd;
	ms_copy(path_cmd, splitted_path.data, splitted_path.size);
	path_cmd = path_cmd + splitted_path.size;
	*path_cmd = '/';
	path_cmd++;
	ms_copy(path_cmd, argv, ms_str_length(argv));
	path_cmd = path_cmd + ms_str_length(argv);
	*path_cmd = '\0';
	return (path_cmd_start);
}

char	*get_absolut_or_relative_path(char *argv)
{
	t_slice	argv_slice;
	char	*path_cmd;

	argv_slice = slice0(argv);
	if (consume(&argv_slice, "./") == 1 || consume(&argv_slice, "../") == 1
		|| ms_str_find(argv, '/') != NULL)
	{
		path_cmd = argv;
		if (access(path_cmd, F_OK) == 0)
			return (path_cmd);
		else
			execution_error(1, argv, 1, errno);
	}
	return (NULL);
}

char	*get_path_cmd(char **argv, t_state *state)
{
	t_var	*path_var;
	t_slice	splitted_path;
	t_slice	rest_path;
	char	*path_cmd;

	path_cmd = get_absolut_or_relative_path(*argv);
	if (path_cmd != NULL)
		return (path_cmd);
	path_var = vars_get(&(state->root_var), slice0("PATH"));
	if (path_var == NULL)
		return (NULL);
	rest_path.data = slice0(path_var->value).data;
	rest_path.size = slice0(path_var->value).size;
	while (rest_path.size != 0)
	{
		split_once(slice0(rest_path.data), begin_delimiter2,
			&splitted_path, &rest_path);
		rest_path = advance(rest_path);
		path_cmd = join_path_cmd(splitted_path, *argv);
		if (access(path_cmd, F_OK) == 0)
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}
