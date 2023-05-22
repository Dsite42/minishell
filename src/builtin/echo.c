/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 16:58:05 by cgodecke          #+#    #+#             */
/*   Updated: 2023/05/20 17:35:54 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/param.h>
#include "../libft.h"

// ft_split is used to imitate the parser
// print_folder_content is for fun and has no proper error handling

int	arr_len(char **arr)
{
	int	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}


void	print_folder_content(int n_flag)
{
	DIR						*dir_stream;
	struct dirent			*directory_entry;
	struct stat	*restrict	statbuf;
	char					buffer[MAXPATHLEN + 1];

	dir_stream = opendir(getcwd(buffer, sizeof(buffer)));
	directory_entry = readdir(dir_stream);
	statbuf = malloc(sizeof(struct stat));
	while (directory_entry != NULL)
	{
		lstat(ft_strjoin(buffer, ft_strjoin("/", directory_entry->d_name)), statbuf);
		if (statbuf->st_mode & __S_IFREG || statbuf->st_mode & __S_IFDIR)
			printf("%s ", directory_entry->d_name);
		directory_entry = readdir(dir_stream);
	}
	if (n_flag == 0)
		printf("\n");
}

int	builtin_echo(int argc, char *argv[], int out_fd, t_state *state)
{
	char			**splitted_argvs;
	int				n_flag;

	n_flag = 0;
	splitted_argvs = ft_split(argv[2], ' ');
	argc = arr_len(splitted_argvs);
	if (argc != 0)
	{
		if (ms_strlen(*splitted_argvs) == 2
			&& ft_strncmp(*splitted_argvs, "-n", 2) == 0)
			n_flag = 1;
	}
	if (argc == 0 || (argc == 1 && n_flag == 1))
	{
		if (n_flag != 1)
			print_fd(out_fd, "\n");
		return (0);
	}
	if (n_flag == 1)
		splitted_argvs++;
	if ((n_flag == 1 && argc == 2 || n_flag == 0 && argc == 1)
		&& **splitted_argvs == '*')
	{
		print_folder_content(n_flag);
		return (0);
	}
	while (*splitted_argvs)
	{
		print_fd(out_fd, "%s", *splitted_argvs);
		splitted_argvs++;
		if (*splitted_argvs)
			print_fd(out_fd, " ");
		else if (n_flag != 1)
			print_fd(out_fd, "\n");
	}
	return (0);
}
