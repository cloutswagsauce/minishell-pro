/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:59:34 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/27 14:09:37 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	path_split_append(t_com *command, t_data *data)
{
	char	**path_split;
	char	*path_value;
	int		has_path;

	handle_direct_path(command, data);
	path_value = getenv("PATH");
	has_path = find_path(data);
	if (!has_path)
	{
		ft_putstr_fd("PATH variable not set dawg\n", 2);
		exit(127);
	}
	path_split = ft_split(path_value, ':');
	if (!path_split)
	{
		ft_putstr_fd("Failed to split PATH\n", 2);
		exit(127);
	}
	try_exec_from_path(path_split, command, data);
	exit(127);
}

int	create_new_arg(int *arg_count, t_com *current_cmd, t_token *cur_token)
{
	char	**temp_argv;

	if (!current_cmd)
		return (1);
	(*arg_count)++;
	temp_argv = malloc(sizeof(char *) * ((*arg_count) + 1));
	if (!temp_argv)
	{
		perror("temp argv failed:");
		return (1);
	}
	if (current_cmd->argv)
		temp_arg_creator(current_cmd, temp_argv, arg_count);
	temp_argv[(*arg_count) - 1] = ft_strdup(cur_token->value);
	temp_argv[(*arg_count)] = NULL;
	arg_helper(cur_token, current_cmd, arg_count);
	current_cmd->argv = temp_argv;
	return (0);
}

int	create_new_command(t_com **current_cmd, int *arg_count, t_token *cur_token)
{
	*current_cmd = malloc(sizeof(t_com));
	if (!*current_cmd)
		return (1);
	(*current_cmd)->argv = malloc(sizeof(char *) * 2);
	(*current_cmd)->argv[0] = ft_strdup(cur_token->value);
	(*current_cmd)->argv[1] = NULL;
	(*current_cmd)->has_inpipe = 0;
	(*current_cmd)->has_outpipe = 0;
	(*current_cmd)->d_quote = 0;
	(*current_cmd)->s_quote = NULL;
	(*current_cmd)->append_output = 0;
	(*current_cmd)->delim = NULL;
	(*current_cmd)->output_file = 0;
	(*current_cmd)->is_builtin = 0;
	(*current_cmd)->next = NULL;
	*arg_count = 1;
	return (0);
}

t_com	*parse_input(char *str)
{
	t_token	*tokens;
	t_com	*commands;
	t_token	*cur_token;
	t_com	*current_cmd;
	int		arg_count;

	tokens = tokenize_input(str);
	cur_token = tokens;
	commands = NULL;
	current_cmd = NULL;
	arg_count = 0;
	while (cur_token)
	{
		printf("treating token: %s\n", cur_token->value);
		if (token_dispatcher(&commands, &current_cmd, &cur_token, &arg_count))
			return (0);
	}
	if (current_cmd)
		current_cmd->is_builtin = is_command_builtin(current_cmd);
	return (commands);
}
