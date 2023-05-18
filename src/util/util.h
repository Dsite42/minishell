/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 15:59:24 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/16 20:02:11 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

int		print_fd(int fd, const char *format, ...);
size_t	ms_strlen(const char *string);
void	*ms_memcpy(void *destination, const void *source, size_t size);

#endif // !UTIL_H