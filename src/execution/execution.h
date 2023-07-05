/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:58:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 15:13:24 by jsprenge         ###   ########.fr       */
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

// run_cmd.c
void	run_cmds(t_cmd *root_cmd, char **envp, t_state *state);
void	create_pipe(int i, int num_cmds, int (*pipefd)[2]);

// get_path_cmd.c
char	*get_path_cmd(char **argv, t_state *state);

// child.c
void	child(t_cmd *root_cmd, char **envp, t_piping *piping_data,
			t_state *state);

// error.c
void	execution_error(int shall_exit, char *message, int isstrerror,
			int exit_code);
void	error_cmd_not_found(char *cmd);

// check_heredoc.c
int		check_heredoc(t_piping piping_data);

// parent.c
int		parent(t_piping *piping_data, t_state *state);
int		is_builtin(char **argv);
int		run_builtin(char **argv, t_state *state);

// input_redirection.c
void	input_redirection(t_piping *piping_data, int *fd_dup);

// output_redirection.c
int		output_redirection(t_piping *piping_data, int *fd_dup);

#endif // !EXECUTION_H