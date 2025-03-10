/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens_part_II.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 23:59:00 by iduric            #+#    #+#             */
/*   Updated: 2025/02/27 15:40:29 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	token_dispatcher(t_com **commands, t_com **current_cmd, t_token **tokens,
		int *arg_count)
{
	t_token	*current;
	int		ret;

	current = *tokens;
	ret = 0;
	if (!current)
		return (0);
	if (current->type == TOKEN_WORD || current->type == TOKEN_SQUOTES
		|| current->type == TOKEN_DQUOTES)
		ret = handle_word_case(commands, current_cmd, tokens, arg_count);
	else if (current->type == TOKEN_PIPE)
		ret = handle_pipe_case(commands, current_cmd, tokens, arg_count);
	else if (current->type == TOKEN_REDIRECT_OUT
		|| current->type == TOKEN_APPEND)
		ret = handle_redirect_case(commands, current_cmd, tokens, arg_count);
	else if (current->type == TOKEN_HEREDOC)
		ret = handle_heredoc_case(commands, current_cmd, tokens, arg_count);
	return (ret);
}

int	init_new_pipe_cmd(t_com *new_cmd)
{
	new_cmd->argv[0] = NULL;
	new_cmd->argv[1] = NULL;
	new_cmd->has_inpipe = 1;
	new_cmd->has_outpipe = 0;
	new_cmd->next = NULL;
	return (0);
}

int	handle_pipe_token(t_com **current_cmd, int *arg_count)
{
	t_com	*new_cmd;

	if (!*current_cmd || !(*current_cmd)->argv || !(*current_cmd)->argv[0])
	{
		ft_printf("syntaxxxx error near unexpected token `|'\n");
		store_exit_status(2);
		return (1);
	}
	(*current_cmd)->has_outpipe = 1;
	new_cmd = malloc(sizeof(t_com));
	if (!new_cmd)
		return (1);
	ft_memset(new_cmd, 0, sizeof(t_com));
	new_cmd->argv = malloc(sizeof(char *) * 2);
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (1);
	}
	init_new_pipe_cmd(new_cmd);
	(*current_cmd)->next = new_cmd;
	(*current_cmd) = new_cmd;
	(*arg_count) = 0;
	return (0);
}

int handle_redirect_token(t_com *current_cmd, t_token **cur_token, int append)
{
    t_token *current;
    t_token *next;
    t_token *to_free;

	current = *cur_token;
	next = current->next;
    if (!next)
    {
        if (current_cmd)
            free_commands(current_cmd);
        free_tokens(*cur_token);
        store_exit_status(127);
        return (1);
    }
    if (current_cmd->output_file)
        free(current_cmd->output_file);
    current_cmd->output_file = ft_strdup(next->value);
    if (append)
        current_cmd->append_output = 1;
    to_free = next;
    *cur_token = next->next;
    free_single_token(to_free);
    return (0); 
}

int	handle_heredoc_token(t_com *current_cmd, t_token *cur_token)
{
	t_token	*temp;

	if (!cur_token || !cur_token->next || !cur_token->next->value)
		return (1);
	if (current_cmd->delim)
	{
		free(current_cmd->delim);
		current_cmd->delim = NULL;
	}
	current_cmd->delim = ft_strdup(cur_token->next->value);
	if (!current_cmd->delim || !(ft_strncmp(current_cmd->delim, "",
				ft_strlen(current_cmd->delim))))
		return (1);
	temp = cur_token->next;
	if (cur_token->value)
		free(cur_token->value);
	cur_token->value = temp->value;
	cur_token->next = temp->next;
	free(temp);
	return (0);
}
