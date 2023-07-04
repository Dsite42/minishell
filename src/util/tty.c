/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:21:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 01:25:39 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <unistd.h>
#include <signal.h>
#include <termios.h>

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

static int	g_flags = 0;

static void	signal_handler(int number)
{
	if (number == SIGINT && tty_get_flag(TTY_HEREDOC))
	{
		tty_set_flag(TTY_HEREDOC, 0);
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
		return ;
	}
	if (tty_get_flag(TTY_IS_CHILD))
	{
		if (number == SIGQUIT)
			write(STDERR_FILENO, "Quit\n", 5);
		else
			write(STDERR_FILENO, "\n", 1);
		return ;
	}
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
	tty_enter(0);
}

void	tty_enter(int is_child)
{
	struct termios	ios;

	tty_set_flag(TTY_IS_CHILD, is_child);
	tcgetattr(STDIN_FILENO, &ios);
	if (is_child)
		ios.c_lflag |= ECHOCTL;
	else
		ios.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &ios);
}

int	tty_get_flag(unsigned int index)
{
	return ((g_flags & (1 << index)) != 0);
}

void	tty_set_flag(unsigned int index, int enable)
{
	if (enable)
		g_flags |= 1 << index;
	else
		g_flags &= ~(1 << index);
}
