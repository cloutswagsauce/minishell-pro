/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 20:32:16 by iduric            #+#    #+#             */
/*   Updated: 2025/02/27 00:22:35 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_word_case(t_com **commands, t_com **current_cmd, t_token **tokens,
		int *arg_count)
{
	t_token	*current;
	t_token	*to_free;
	int		ret;

	current = *tokens;
	ret = 0;
	handle_word_tokens(commands, current_cmd, current, arg_count);
	to_free = current;
	*tokens = current->next;
	free_single_token(to_free);
	return (ret);
}

int	handle_pipe_syntax_error(t_com **commands)
{
	int	ret;

	ret = 1;
	if (*commands)
		free_commands(*commands);
	ft_printf("syntadddx error near unexpected token `|'\n");
	store_exit_status(2);
	return (ret);
}

int	handle_pipe_case(t_com **commands, t_com **current_cmd, t_token **tokens,
		int *arg_count)
{
	t_token	*current;
	t_token	*to_free;
	int		ret;

	current = *tokens;
	ret = handle_pipe_token(current_cmd, arg_count);
	if (ret)
	{
		if (*commands)
			free_commands(*commands);
		free_tokens(*tokens);
		*commands = NULL;
		*tokens = NULL;
		return (ret);
	}
	to_free = current;
	*tokens = current->next;
	free_single_token(to_free);
	if (!*tokens)
		return (handle_pipe_syntax_error(commands));
	return (ret);
}

int	handle_redirect_case(t_com **commands, t_com **current_cmd,
		t_token **tokens, int *arg_count)
{
	t_token	*current;
	t_token	*to_free;
	int		ret;

	current = *tokens;
	ret = 0;
	if (!*current_cmd)
	{
		create_new_command(current_cmd, arg_count, current);
		if (!*commands)
			*commands = *current_cmd;
	}
	ret = handle_redirect_token(*current_cmd, tokens,
			(current->type == TOKEN_APPEND));
	if (!ret)
	{
		to_free = current;
		free_single_token(to_free);
	}
	return (ret);
}

int	handle_heredoc_case(t_com **commands, t_com **current_cmd, t_token **tokens,
		int *arg_count)
{
	t_token	*current;
	t_token	*to_free;
	int		ret;

	current = *tokens;
	ret = 0;
	if (!*current_cmd)
	{
		create_new_command(current_cmd, arg_count, current);
		if (!*commands)
			*commands = *current_cmd;
	}
	handle_heredoc_token(*current_cmd, current);
	to_free = current;
	*tokens = current->next;
	free_single_token(to_free);
	return (ret);
}
