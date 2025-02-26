/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:32:02 by iduric            #+#    #+#             */
/*   Updated: 2025/02/26 19:04:52 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	error_token_parsing(t_token **tokens)
{
	printf("syntax error near unexpected token\n");
	store_exit_status(2);
	free_tokens(*tokens);
	*tokens = NULL;
	return ;
}

int	is_operator_start(char *input, int *i)
{
	return (*i == 0 || isspace((char)input[*i - 1]) || input[*i] == '|'
		|| input[*i] == '<' || input[*i] == '>');
}

void	handle_pipe(char *input, int *i, t_token **tokens)
{
	t_token	*pipe_token;
	t_token	*temp;

	if (input[*i + 1] == '|')
		return error_token_parsing(tokens);
	pipe_token = new_token("|", TOKEN_PIPE, 0);
	if (pipe_token)
	{
		if (!*tokens)
			*tokens = pipe_token;
		else
		{
			temp = *tokens;
			while (temp->next)
				temp = temp->next;
			temp->next = pipe_token;
		}
	}
	(*i)++;
}

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