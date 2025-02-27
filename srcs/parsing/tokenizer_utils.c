/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:34:39 by iduric            #+#    #+#             */
/*   Updated: 2025/02/27 01:25:15 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*create_redirect_it_token(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	token = NULL;
	if (input[*i + 1] == '<')
	{
		if (input[*i + 2] == '<')
		{
			error_token_parsing(tokens);
			return (NULL);
		}
		token = new_token("<<", TOKEN_HEREDOC, 0);
		(*i) += 2;
	}
	else
	{
		if (input[*i + 1] == '<')
		{
			error_token_parsing(tokens);
			return (NULL);
		}
		token = new_token("<", TOKEN_REDIRECT_IN, 0);
		(*i)++;
	}
	return (token);
}

void	append_token_to_list(t_token **tokens, t_token *token)
{
	t_token	*temp;

	if (!token)
		return ;
	if (!*tokens)
		*tokens = token;
	else
	{
		temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = token;
	}
}

void	handle_redirect_it(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	token = create_redirect_it_token(input, i, tokens);
	append_token_to_list(tokens, token);
}

void	handle_redirect_ot(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	if (input[*i + 1] == '>')
	{
		if (input[*i + 2] == '>')
			return (error_token_parsing(tokens));
		token = new_token(">>", TOKEN_APPEND, 0);
		(*i) += 2;
	}
	else
	{
		token = new_token(">", TOKEN_REDIRECT_OUT, 0);
		(*i)++;
	}
	if (token && !*tokens)
		*tokens = token;
	else if (token && *tokens)
		append_token_to_list(tokens, token);
}

void	handle_non_operator(char *input, int *i, t_token **tokens)
{
	char	*word;

	word = handle_word(input, i);
	if (word)
		add_token(tokens, word, TOKEN_WORD, 1);
}
