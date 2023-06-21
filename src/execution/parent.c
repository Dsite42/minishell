/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:22:16 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/21 11:24:10 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>

void	parent(t_piping *piping_data)
{
	if (piping_data->i > 0)
		close(piping_data->prev_read);
	if (piping_data->i < piping_data->num_cmds - 1)
	{
		close(piping_data->pipefd[1]);
		piping_data->prev_read = piping_data->pipefd[0];
	}
}
