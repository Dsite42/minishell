/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chris <chris@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:16:34 by chris             #+#    #+#             */
/*   Updated: 2023/05/23 16:39:49 by chris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/history.h>
#include "readline/readline.h"
#include "state/state.h"

// ctrl-\ does anyway nothing.

void	handle_signals(int sig)
{
	(void)sig;
	if (sig == SIGINT)
		write(0, "\nminishell> ", 12);
	else if (sig == SIGTSTP)
	{
		//vars_clr(&state.root_var); should be state as global?
		rl_clear_history();
		exit(0);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	struct sigaction	sa;

	(void) argc;
	(void) argv;
	(void) envp;
	sa.sa_handler = &handle_signals;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
			break ;
		add_history(input);
		free(input);
	}
	return (0);
}
