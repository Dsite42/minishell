/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 19:40:37 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/30 18:30:56 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state/state.h"
#include "parser/parser.h"
#include "builtin/builtin.h"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void	run_cmds(t_cmd *root_cmd, char **envp, t_state *state);

static t_result	handle_line(char *line, t_state *state, char **envp)
{
	t_result	result;
	t_cmd		*root_cmd;
	t_word		*root_word;

	add_history(line);
	result = words_from_slice(&root_word, slice0(line));
	if (result == S_OK)
	{
		result = cmds_from_words(root_word, &root_cmd, state);
		if (result == S_OK)
		{
			run_cmds(root_cmd, envp, state);
			cmd_clear(&root_cmd);
		}
		word_clear(&root_word);
	}
	free(line);
	return (result);
}

static void	signal_handler(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
	{
		if (sig == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
		}
		rl_redisplay();
	}
}

static void	setup_signals(void)
{
	struct sigaction	sa;
	struct termios		term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	ms_zero(&sa, sizeof(sa));
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_state				state;
	t_result			result;
	char				*line;

	(void) argc;
	(void) argv;
	state.saved_STDOUT_FILENO = dup(1);
	if (!vars_from_envp(envp, &state.root_var))
		return (1);
	setup_signals();
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			break ;
		add_history(line);
		result = handle_line(line, &state, envp);
		if (result != S_OK)
			print_fd(STDERR_FILENO, "Error code %i while parsing\n", result);
	}
	write(STDERR_FILENO, "exit\n", 5);
	state_drop(&state);
	return (0);
}
