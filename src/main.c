/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:36:07 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/15 16:27:55 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	run_cmds(char **argv, char **envp, t_state *state);

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









char	*of_two_strjoin(char *str1, char *str2, char *sep)
{
	char	*joined;
	char	*joined_start;
	int		len_join;

	len_join = ms_str_length(str1) + ms_str_length(str2)
		+ ms_str_length(sep) + 1;
	joined = (char *) malloc(len_join * sizeof(char));
	joined_start = joined;
	if (joined == NULL)
		return (NULL);
	ms_copy((void *)joined, (void *)str1, ms_str_length(str1));
	joined = joined + ms_str_length(str1);
	ms_copy(joined, sep, ms_str_length(sep));
	joined = joined + ms_str_length(sep);
	ms_copy(joined, str2, ms_str_length(str2));
	joined = joined + ms_str_length(str2);
	*(joined) = '\0';
	return (joined_start);
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
			*envp = of_two_strjoin(head_var->name, head_var->value, "="); //
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
	run_cmds(new_argv, envp, state);
	//run_builtin(new_argv, state);
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
