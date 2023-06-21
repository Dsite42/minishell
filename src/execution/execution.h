/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:58:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/21 11:59:57 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../state/state.h"
# include "../parser/parser.h"

typedef struct s_piping
{
	int		pipefd[2];
	int		i;
	int		prev_read;
	t_cmd	*cmd;
	int		num_cmds;
}	t_piping;

void	run_cmds(char **argv, char **envp, t_state *state);
char	*get_path_cmd(char **argv, t_state *state);
void	child(char **envp, t_piping *piping_data, t_state *state);
void	pipex_error(int shall_exit, char *message, int isstrerror,
			int exit_code);
void	check_heredoc(t_piping piping_data);
void	create_pipe(int i, int num_cmds, int (*pipefd)[2]);
void	parent(t_piping *piping_data);
void	input_redirection(t_piping *piping_data, int *fd_dup);

#endif // !EXECUTION_H