/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:02:08 by jsprenge          #+#    #+#             */
/*   Updated: 2023/07/05 13:39:39 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

#include <limits.h>

#define CAPACITY_STEP 1024

int	strbuf_init(t_strbuf *self)
{
	char	*new_data;

	new_data = malloc(CAPACITY_STEP);
	if (new_data == NULL)
		return (0);
	new_data[0] = '\0';
	self->data = new_data;
	self->length = 1;
	self->capacity = CAPACITY_STEP;
	return (1);
}

static int	grow_buffer(t_strbuf *self, t_slice slice)
{
	char	*new_data;
	size_t	difference;
	size_t	new_capacity;

	difference = slice.size - (self->capacity - self->length);
	if (SIZE_MAX - self->capacity < difference)
		return (0);
	new_capacity = self->capacity + difference;
	if (SIZE_MAX - new_capacity < CAPACITY_STEP - 1)
		return (0);
	new_capacity += CAPACITY_STEP - 1;
	new_capacity -= new_capacity % CAPACITY_STEP;
	new_data = malloc(new_capacity);
	if (new_data == NULL)
		return (0);
	ms_copy(new_data, self->data, self->length);
	free(self->data);
	self->data = new_data;
	self->capacity = new_capacity;
	return (1);
}

int	strbuf_push(t_strbuf *self, t_slice slice)
{
	if (self->data == NULL || self->capacity == 0)
		if (!strbuf_init(self))
			return (0);
	if (self->capacity - self->length < slice.size)
		if (!grow_buffer(self, slice))
			return (0);
	ms_copy(&self->data[self->length - 1], slice.data, slice.size);
	self->length += slice.size;
	self->data[self->length] = '\0';
	return (1);
}

void	strbuf_drop(t_strbuf *self)
{
	free(self->data);
	self->data = NULL;
	self->length = 0;
	self->capacity = 0;
}

void	strbuf_take(t_strbuf *self, char **p_string)
{
	*p_string = self->data;
	self->data = NULL;
	self->capacity = 0;
	self->length = 0;
}
