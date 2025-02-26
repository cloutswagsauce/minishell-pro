/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:34:39 by iduric            #+#    #+#             */
/*   Updated: 2025/02/25 20:36:44 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_operator_start(char *input, int *i)
{
	return (*i == 0 || 
			isspace((char)input[*i - 1]) || 
			input[*i] == '|' || 
			input[*i] == '<' || 
			input[*i] == '>');
}

void	handle_pipe(char *input, int *i, t_token **tokens)
{
	t_token	*pipe_token;
	t_token	*temp;

	// Check for multiple consecutive pipes
	if (input[*i + 1] == '|')
	{
		printf("syntax error near unexpected token `|'\n");
		store_exit_status(2);
		*tokens = NULL;  // Clear tokens to abort parsing
		return;
	}
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

void	handle_redirect_it(char *input, int *i, t_token **tokens)
{
	t_token	*token;
	t_token	*temp;

	if (input[*i + 1] == '<')
	{
		// Check for more than two consecutive '<'
		if (input[*i + 2] == '<')
		{
			printf("syntax error near unexpected token `<'\n");
			store_exit_status(2);
			*tokens = NULL;  // Clear tokens to abort parsing
			return;
		}
		token = new_token("<<", TOKEN_HEREDOC, 0);
		(*i) += 2;
	}
	else
	{
		// Check for single '<' followed by '<'
		if (input[*i + 1] == '<')
		{
			printf("syntax error near unexpected token `<'\n");
			store_exit_status(2);
			*tokens = NULL;  // Clear tokens to abort parsing
			return;
		}
		token = new_token("<", TOKEN_REDIRECT_IN, 0);
		(*i)++;
	}
	if (token)
	{
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
}

void	handle_redirect_ot(char *input, int *i, t_token **tokens)
{
	t_token	*token;
	t_token	*temp;

	if (input[*i + 1] == '>')
	{
		// Check for more than two consecutive '>'
		if (input[*i + 2] == '>')
		{
			
			printf("syntax error near unexpected token `>'\n");
			store_exit_status(2);
			*tokens = NULL; 
			 // Clear tokens to abort parsing
			return;
		}
		token = new_token(">>", TOKEN_APPEND, 0);
		(*i) += 2;
	}
	else
	{
		// Check for single '>' followed by '>'
		if (input[*i + 1] == '>')
		{
			printf("syntax error near unexpected token `>'\n");
			store_exit_status(2);
			*tokens = NULL;  // Clear tokens to abort parsing
			return;
		}
		token = new_token(">", TOKEN_REDIRECT_OUT, 0);
		(*i)++;
	}
	if (token)
	{
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
}

void	handle_non_operator(char *input, int *i, t_token **tokens)
{
	char *word = handle_word(input, i);
	if (word)
		add_token(tokens, word, TOKEN_WORD, 1);
}

/*void	handle_operators(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
		handle_pipe(input, i, tokens);
	else if (input[*i] == '<')
		handle_redirect_it(input, i, tokens);
	else if (input[*i] == '>')
		handle_redirect_ot(input, i, tokens);
	else
		handle_non_operator(input, i, tokens);
}*/