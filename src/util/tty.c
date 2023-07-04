/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:21:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/04 16:27:50 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <unistd.h>
#include <signal.h>
#include <termios.h>

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

static void	signal_handler(int number)
{
	if (number == SIGINT || number == SIGQUIT)
	{
		if (number == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_replace_line("", 1);
			rl_on_new_line();
		}
		rl_redisplay();
	}
}

void	tty_setup(void)
{
	struct sigaction	action;

	ms_zero(&action, sizeof(action));
	action.sa_handler = signal_handler;
	action.sa_flags = SA_RESTART;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	tty_set_echo(0);
}

void	tty_set_echo(int enable)
{
	struct termios	ios;

	tcgetattr(STDIN_FILENO, &ios);
	if (enable)
		ios.c_lflag |= ECHOCTL;
	else
		ios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &ios);
}
