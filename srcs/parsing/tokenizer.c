/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:34:49 by iduric            #+#    #+#             */
/*   Updated: 2025/02/25 20:36:52 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_word(char *input, int *i, t_token **tokens, char **buf)
{
	char	*temp;
	char	*new_buf;

	temp = handle_word(input, i);
	if (!temp)
		return (1);
	if (!*buf)
		*buf = temp;
	else
	{
		new_buf = ft_strjoin(*buf, temp);
		free(*buf);
		free(temp);
		*buf = new_buf;
	}
	if (input[*i] && !isspace((char)input[*i]) && (input[*i] == '"'
			|| input[*i] == '\''))
		return (1);
	if (*buf)
	{
		add_token(tokens, *buf, TOKEN_WORD, 1);
		*buf = NULL;
	}
	return (1);
}

void	init_tokenize_vars(t_token **tokens, char **buf, int *i)
{
	*tokens = NULL;
	*buf = NULL;
	*i = 0;
}

int	process_next_char(char *input, int *i, t_token **tokens, char **buf)
{
	if (input[*i] == '"' || input[*i] == '\'')
	{
		if (!process_quoted_string(input, i, tokens, buf))
			return (0);
	}
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		handle_operators(input, i, tokens);
		if (*tokens == NULL)  // Check if tokens were cleared due to syntax error
			return (0);
	}
	else
	{
		if (!process_word(input, i, tokens, buf))
			return (0);
	}
	return (1);
}

void	handle_remaining_buffer(t_token **tokens, char **buf)
{
	if (*buf)
		add_token(tokens, *buf, TOKEN_WORD, 1);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	int		i;
	char	*buf;

	init_tokenize_vars(&tokens, &buf, &i);
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		if (!process_next_char(input, &i, &tokens, &buf))
		{
			free_tokens(tokens);
			free(buf);
			return (NULL);
		}
	}
	handle_remaining_buffer(&tokens, &buf);
	return (tokens);
}
