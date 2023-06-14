/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:10:00 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/14 15:09:05 by cgodecke         ###   ########.fr       */
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




size_t	begin_delimiter2(t_slice slice)
{
	return (slice.size > 0 && *slice.data == ':');
}

char	*join_path_cmd(t_slice splitted_path, char *argv)
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

char	*get_path_cmd(char **argv, t_state *state)
{
	t_var	*path_var;
	t_slice	splitted_path;
	t_slice	rest_path;
	char	*to_join[3];
	char	*path_cmd;

	path_var = vars_get(&(state->root_var), slice0("PATH"));

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
	}
	return (NULL);
}

void	pipex_error(int shall_exit, char *message,
			int isstrerror, int exit_code)
{
	if (isstrerror == 1)
		print_fd(2, "pipex: %s %s\n", message, strerror(exit_code));
	else
		print_fd(2, "pipex: %s\n", message);
	if (shall_exit == 1)
		exit(exit_code);
}


void	child(char **argv, char **envp, int *pipefd, t_state *state, int nbr)
{
	int		fd_dup[2];
	int		fd_in;
	char	*path_cmd;

	path_cmd = get_path_cmd(argv, state);
	if (nbr == 1)
	{
		print_fd(0, "path:%s\n", path_cmd);
		fd_dup[0] = dup2 (pipefd[1], STDOUT_FILENO);
		if (fd_dup[0] == -1)
			pipex_error(1, "dup21 error", 1, errno);
		close(pipefd[0]);
		close(pipefd[1]);
		//fd_dup[1] = dup2 (fd_in, STDIN_FILENO);
		//if (fd_dup[1] == -1)
		//	pipex_error(1, "dup21 error", 1, errno);
		//close(fd_in);
	}
	else if (nbr == 2)
	{
		print_fd(0, "path2:%s\n", path_cmd);
		fd_dup[0] = dup2 (pipefd[0], STDIN_FILENO);
		if (fd_dup[0] == -1)
			pipex_error(1, "dup22 error", 1, errno);
		//close(pipefd[0]);
		close(pipefd[1]);


//char buffer[4096]; // Buffer for reading from the pipe
//        ssize_t num_read; // Number of bytes read from the pipe
//
//        while ((num_read = read(fd_dup[0], buffer, sizeof(buffer))) > 0)
//        {
//            if (write(STDOUT_FILENO, buffer, num_read) == -1)
//            {
//                pipex_error(1, "write error", 1, errno);
//            }
//        }	

		//int stdout_fd = dup(STDOUT_FILENO);

		//fd_dup[1] = dup2 (STDOUT_FILENO, STDOUT_FILENO);
		//fd_dup[1] = dup2(stdout_fd, STDOUT_FILENO);
		//if (fd_dup[1] == -1)
		//	pipex_error(1, "dup2 error", 1, errno);
		//close(fd_in);
		// close(stdout_fd);
	}
	//if (cmd_list->cmd_path != NULL)
	//{
		if (execve((const char *) path_cmd, 
				argv, envp) == -1)
				{
					pipex_error(0, "execve child error.", 1, errno);
				}
	//}
}



void	run_cmds(char **argv, char **envp, t_state *state)
{
    int num_cmds = ms_ptrs_count((void *)argv);  // Number of commands specified in argv

    int prev_read = STDIN_FILENO;  // Input source for the first command
    int pipefd[2];  // Pipe file descriptors
	char	*path_cmd;



    int i = 1;
    while (i <= num_cmds)
	{
        if (i < num_cmds)
		{
            // Create a pipe for commands except the last one
            if (pipe(pipefd) == -1)
			{
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Fork a child process
        pid_t pid = fork();
        if (pid == -1) 
		{
            perror("fork");
            exit(EXIT_FAILURE);
        } 
		else if (pid == 0)
		{
            // Child process
			int		fd_dup[2];

            // Set up input redirection
            if (i > 1)
			{
                // Redirect stdin to the read end of the previous pipe
                fd_dup[0] = dup2(prev_read, STDIN_FILENO);
				if (fd_dup[0] == -1)
					pipex_error(1, "dup21 error", 1, errno);
                close(prev_read);
            }

            // Set up output redirection
            if (i < num_cmds)
			{
                // Redirect stdout to the write end of the current pipe
                fd_dup[1] = dup2(pipefd[1], STDOUT_FILENO);
				if (fd_dup[1] == -1)
					pipex_error(1, "dup21 error", 1, errno);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Execute the command
			char *argv_child[2];
			argv_child[0] = argv[i-1];
			argv_child[1] = NULL;
			//exit(0);
			path_cmd = get_path_cmd(argv_child, state);
			print_fd(1, "path_cmd:%s", path_cmd);
			if (execve((const char *) path_cmd, 
					argv_child, envp) == -1)
			{
				pipex_error(0, "execve child error.", 1, errno);
			}

			// execvp(*argv_child, argv_child);
            // If execvp returns, an error occurred
            perror("execvp");
            exit(EXIT_FAILURE);
        } else
		{
            // Parent process

            // Close unnecessary pipe ends
            if (i > 1)
			{
                close(prev_read);
            }
            if (i < num_cmds)
			{
                close(pipefd[1]);
                prev_read = pipefd[0];
            }
        }

        i++;
    }

    // Wait for all child processes to finish
    while (num_cmds > 0) {
        int status;
        wait(&status);
        num_cmds--;
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
