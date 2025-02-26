/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:32:02 by iduric            #+#    #+#             */
/*   Updated: 2025/02/25 20:32:10 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_quotes(char *input, int *i, t_token **tokens)
{
	int		start;
	char	quote;
	char	*buf;

	if (input[*i] != '"' && input[*i] != '\'')
		return (1);
	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_printf("you forgot to close the damn quote!\n");
		return (0);
	}
	buf = ft_substr(input, start, *i - start);
	if (!buf)
		return (0);
	if (quote == '\'')
		add_token(tokens, buf, TOKEN_SQUOTES, 1);
	else
		add_token(tokens, buf, TOKEN_DQUOTES, 1);
	(*i)++;
	
	// Check if next character is another quote
	if (input[*i] && (input[*i] == '"' || input[*i] == '\''))
		return (handle_quotes(input, i, tokens));
	return (1);
}

void	handle_word_tokens(t_com **commands, t_com **current_cmd,
		t_token *tokens, int *arg_count)
{
	if (!*current_cmd)
	{
		create_new_command(current_cmd, arg_count, tokens);
		if (!*commands)
			*commands = *current_cmd;
	}
	else
		create_new_arg(arg_count, *current_cmd, tokens);
}