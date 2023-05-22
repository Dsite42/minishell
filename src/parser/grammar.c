/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsprenge <jsprenge@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 02:14:45 by jsprenge          #+#    #+#             */
/*   Updated: 2023/05/22 02:21:44 by jsprenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private.h"

int	is_space(char input)
{
	return (input == ' ' || input == '\n' || input == '\r'
		|| input == '\v' || input == '\t' || input == '\f');
}

int	is_not_identifier(char input)
{
	return ((input < 'a' || input > 'z') && (input < 'A' || input > 'Z')
		&& input != '_');
}

int	is_word_split(char input)
{
	return (is_space(input) || input == '\'' || input == '"' || input == '$');
}

int	is_single_quote_split(char input)
{
	return (input == '\'');
}

int	is_double_quote_split(char input)
{
	return (input == '"' || input == '$');
}
