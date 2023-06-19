/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:58:30 by jsprenge          #+#    #+#             */
/*   Updated: 2023/06/19 15:19:33 by cgodecke         ###   ########.fr       */
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

#endif // !EXECUTION_H