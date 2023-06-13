/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/13 17:58:34 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util/util.h"
#include "state/state.h"
#include "parser/parser.h"
#include "builtin/builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>


#include <string.h>

static int	run_builtin(char **argv, t_state *state)
{
	int	count;

	count = (int) ms_ptrs_count(argv);
	if (ms_str_compare(argv[0], "cd", 0) == 0)
		return (builtin_cd(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "echo", 0) == 0)
		return (builtin_echo(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "env", 0) == 0)
		return (builtin_env(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "exit", 0) == 0)
		return (builtin_exit(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "export", 0) == 0)
		return (builtin_export(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "pwd", 0) == 0)
		return (builtin_pwd(count, argv, STDOUT_FILENO, state));
	if (ms_str_compare(argv[0], "unset", 0) == 0)
		return (builtin_unset(count, argv, STDOUT_FILENO, state));
	print_fd(STDERR_FILENO, "minishell: %s: command not found\n", argv[0]);
	return (127);
}


///// ft_split starts
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*(s + i) != '\0')
	{
		i++;
	}
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	length;

	i = 0;
	length = ft_strlen(src);
	while (size > 0 && i < size - 1 && i < length)
	{
		dst[i] = src[i];
		i++;
	}
	if (size > 0)
	{
		dst[i] = '\0';
	}
	return (length);
}


char	*ft_strdup(const char *s)
{
	char	*dup;
	char	*dup_start;

	dup = malloc((ft_strlen((char *)s) * sizeof(char)) + 1);
	if (dup == NULL)
		return (NULL);
	dup_start = dup;
	while (*s)
	{
		*dup = *s;
		s++;
		dup++;
	}
	*(dup) = '\0';
	return (dup_start);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;

	if (!s)
		return (0);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, s + start, len + 1);
	return (ret);
}

static unsigned int	str_count(char const *str, char c)
{
	unsigned int	i;
	unsigned int	cnt;

	i = 0;
	cnt = 0;
	if (str[0] == '\0')
		return (cnt);
	while (str[i] == c)
		i++;
	if (str[i] == '\0')
		return (cnt);
	while (str[i] != '\0')
	{
		if ((str[i] == c) && (str[i + 1] != c && str[i + 1] != '\0'))
			cnt++;
		i++;
	}
	return (cnt + 1);
}

static unsigned int	betweenlen(char const *str, char sep)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != sep)
		i++;
	return (i);
}

static unsigned int	sea_start(char const *s, char c)
{
	unsigned int	i;

	i = 0;
	if (c == '\0')
		return (i);
	while (s[i] == c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char			*cs;
	char			**arr_split;
	unsigned int	arr_size;
	unsigned int	i;

	i = 0;
	cs = (char *)s;
	arr_size = str_count(cs, c) + 1;
	arr_split = (char **) malloc(arr_size * sizeof(char *));
	if (arr_split == NULL)
		return (NULL);
	cs = cs + sea_start(s, c);
	while (i < arr_size - 1)
	{
		arr_split[i] = ft_substr((char const *) cs, 0, betweenlen(cs, c));
		if (i < arr_size - 2)
			cs = cs + betweenlen(cs, c) + 1
				+ sea_start(cs + betweenlen(cs, c) + 1, c);
		i++;
	}
	arr_split[i] = NULL;
	return (arr_split);
}


///// ft_split ends
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	i = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (!size)
		return (src_len);
	while (src[i] != '\0' && dst_len + i < size - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	if (dst_len > size)
		return (src_len + size);
	return (src_len + dst_len);
}

char	*of_nstrjoin(int n, char **strs, char *sep)
{
	char	*joined;
	int		len_join;
	int		i;

	i = 0;
	len_join = 0;
	while (i < n && strs[i] != NULL)
	{
		len_join = len_join + ft_strlen(strs[i]);
		i++;
	}
	len_join = len_join + ((n -1) * ft_strlen(sep) + 1);
	joined = (char *) malloc(len_join * sizeof(char));
	if (joined == NULL)
		return (NULL);
	joined[0] = '\0';
	i = 0;
	while (i < n && strs[i] != NULL)
	{
		ft_strlcat(joined, strs[i], len_join * sizeof(char));
		if (i != n - 1)
			ft_strlcat(joined, sep, len_join * sizeof(char));
		i++;
	}
	return (joined);
}


size_t	begin_delimiter2(t_slice slice)
{
	return (slice.size > 0 && *slice.data == ':');
}

char *join_path_cmd(t_slice splitted_path, char *argv)
{
	char	*path_cmd;
	char	*path_cmd_start;

	path_cmd = (char *)malloc((splitted_path.size + 1 + ms_str_length(argv) + 1) * sizeof(char));
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

char *get_path_cmd(char **argv, t_state *state)
{
	//char	**splitted_path;
	//char	*to_join[3];
	//char	*path_cmd;
	//t_var	*head_var;

	t_var	*path_var;
	t_slice	splitted_path;
	t_slice	rest_path;
	char	*to_join[3];
	char	*path_cmd;

	path_var = vars_get(&(state->root_var), slice0("PATH"));

	//split_once(slice0(path_var->value), begin_delimiter2, &splitted_path, &rest_path);
	//rest_path = advance(rest_path);
	rest_path.data = slice0(path_var->value).data;
	rest_path.size = slice0(path_var->value).size;
	//print_fd(1, "splitted_path:%a\n", splitted_path);
	//print_fd(1, "rest_path:%a\n", rest_path);
	while (rest_path.size != 0)
	{
		//print_fd(1, "path_var:%s\n", path_var->value);
		//print_fd(1, "test:%i\n", begin_delimiter2(slice0(path_var->value)));
		split_once(slice0(rest_path.data), begin_delimiter2, &splitted_path, &rest_path);
		rest_path = advance(rest_path);
		//print_fd(1, "splitted_path:%a\n", splitted_path);
		//split_once(slice0(rest_path.data), begin_delimiter2, &splitted_path, &rest_path);
		//print_fd(1, "splitted_path:%a\n", splitted_path);
		//print_fd(1, "rest_path:%a\n", rest_path);
		//break;
		path_cmd = join_path_cmd(splitted_path, *argv);
		//exit(0);
		//to_join[0] = splitted_path.data;
		//to_join[1] = *argv;
		//to_join[2] = NULL;
		//path_cmd = of_nstrjoin(2, to_join, "/");
		//print_fd(1, "path_cmdget:%a\n", path_cmd);
		if (access(path_cmd, F_OK) == 0)
			return (path_cmd);
	}





	//head_var = state->root_var;
	//while (head_var != NULL)
	//{
	//	if (ms_str_compare(head_var->name, "PATH", 4) == 0)
	//	{
	//		splitted_path = ft_split(head_var->value, ':');
	//		while (*splitted_path != NULL)
	//		{
	//			to_join[0] = *splitted_path;
	//			to_join[1] = *argv;
	//			to_join[2] = NULL;
	//			path_cmd = of_nstrjoin(2, to_join, "/");
	//			if (access(path_cmd, F_OK) == 0)
	//				return (path_cmd);
	//			splitted_path++;
	//		}
	//	}
	//	head_var = head_var->next;
	//}
	return (NULL);
}

void	child(char **argv, char **envp, int *pipefd)
{
	int	fd_dup[2];
	int	fd_in;
	print_fd(0, "path:%s\n", get_path_cmd(argv, 0));
	//fd_dup[0] = dup2 (pipefd[1], STDOUT_FILENO);
	//if (fd_dup[0] == -1)
	//	pipex_error(1, "dup2 error", 1, errno);
	//close(pipefd[0]);
	//close(pipefd[1]);
	//fd_dup[1] = dup2 (fd_in, STDIN_FILENO);
	//if (fd_dup[1] == -1)
	//	pipex_error(1, "dup2 error", 1, errno);
	//close(fd_in);
	//if (cmd_list->cmd_path != NULL)
	//{
		if (execve((const char *) "/bin/echo", 
				argv, envp) == -1)
				{
					//pipex_error(0, "execve child error.", 1, errno);
				}
	//}
}


void	run_cmds(char **argv, char **envp, t_state *state)
{
	int		pipefd[2];
	pid_t	pid;
	int		stat_loc;

	while (*argv != NULL)
	{
		if (pipe(pipefd) == -1)
		{
			print_fd(0, "error while creating pipe\n");
			exit(0);
		}
		pid = fork();
		if (pid == -1)
		{
			print_fd(0, "error while forking\n");
			exit(0);
		}
		if (pid == 0)
		{
			//print_fd(0, "CHILD\n");
			child(argv, envp, pipefd);
		}
		else
		{
			waitpid(pid, &stat_loc, WNOHANG);
			if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) != 0)
				exit(WEXITSTATUS(stat_loc));
			if (pid > 0)
			{
				print_fd(0, "PARENT argv:%s\n", *argv);
				//parent(cmd_list, argv, envp, pipefd);
				argv = argv + 2;
				print_fd(0, "PARENT2 argv:%s\n", *argv);
			}
		}
	}
}

char	**crate_envp(t_state *state)
{
	t_var	*head_var;
	char	**envp;
	char	**envp_start;
	int		envs_counter;

	envs_counter = 0;
	head_var = state->root_var;
	while (head_var != NULL)
	{
		if (head_var->flags & VAR_EXPORT) // should be VAR_EXPLICITEMPTY but forgot to merge the new master I guess
			envs_counter++;
		head_var = head_var->next;
	}
	envp = (char **)malloc((envs_counter + 1) * sizeof (char *));
	if (envp == NULL)
		return (NULL);
	envp_start = envp;
	head_var = state->root_var;
	while (head_var != NULL)
	{
		if (head_var->flags & VAR_EXPORT)
		{
			*envp = head_var->value;
			//print_fd(0, "TESTinside:%s", *(envp));
			envp++;
		}
		head_var = head_var->next;
	}
	*envp = NULL;
	return (envp_start);
}


static t_result	handle_line(char *line, t_state *state)
{
	t_word		*root_word;
	char		**new_argv;
	t_result	result;
	char		**envp;

	result = word_chain_from_string(&root_word, slice0(line));
	if (result != S_OK)
	{
		free(line);
		return (result);
	}
	add_history(line);
	new_argv = argv_from_word_group(root_word, &state->root_var);
	words_clr(&root_word);
	free(line);
	if (new_argv == NULL)
		return (E_NOMEM);


	//print_fd(1, "handle argv:%s\n", new_argv[0]);
	//print_fd(0, "handle argv:%s\n", new_argv[1]);
	//print_fd(0, "handle argv:%s\n", new_argv[2]);

	envp = crate_envp(state);
	//print_fd(0, "TEST:%s", *(envp+1));
	//while (*envp != NULL)
	//{
	//	print_fd(0, "env:%s\n", *envp);
	//	envp++;
	//}
		print_fd(0, "path:%s\n", get_path_cmd(new_argv, state));
	exit(0);
	run_cmds(new_argv, envp, state);
	run_builtin(new_argv, state);
	ms_ptrs_free(new_argv);
	return (S_OK);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state		state;
	t_result	result;
	char		*line;

	(void) argc;
	(void) argv;
	if (!vars_from_envp(envp, &state.root_var))
		return (1);
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break ;
		add_history(line);
		result = handle_line(line, &state);
		if (result != S_OK)
			print_fd(STDERR_FILENO, "Error code %i while parsing\n", result);
	}
	state_drop(&state);
	return (0);
}
