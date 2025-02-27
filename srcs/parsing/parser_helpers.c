/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:56:01 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/27 14:16:20 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	expand_command_variable(t_com *command, t_data *data, t_list *vars,
		char *expanded_cmd)
{
	expanded_cmd = NULL;
	if (command->argv[0] && command->argv[0][0] == '$')
	{
		expanded_cmd = getenv(command->argv[0] + 1);
		if (!expanded_cmd)
		{
			vars = data->local_env;
			while (vars)
			{
				if (!ft_memcmp(command->argv[0] + 1, vars->name,
						ft_strlen(vars->name)))
				{
					expanded_cmd = vars->value;
					break ;
				}
				vars = vars->next;
			}
		}
		if (expanded_cmd)
		{
			free(command->argv[0]);
			command->argv[0] = ft_strdup(expanded_cmd);
		}
	}
}

void	handle_cmd_not_found(char **path_split, t_com *command)
{
	free_double(path_split);
	if (!ft_strncmp(command->argv[0], ">>", ft_strlen(command->argv[0]))
		|| !ft_strncmp(command->argv[0], ">", ft_strlen(command->argv[0])))
	{
		store_exit_status(0);
		return ;
	}
	ft_putstr_fd("Command not found: ", 2);
	ft_putendl_fd(command->argv[0], 2);
	store_exit_status(127);
}

void	try_exec_from_path(char **path_split, t_com *command, t_data *data)
{
	char	*exec_path;
	char	**current_path_split;
	char	*expanded_cmd;
	int		len;
	t_list	*vars;

	vars = NULL;
	expanded_cmd = NULL;
	expand_command_variable(command, data, vars, expanded_cmd);
	current_path_split = path_split;
	while (*current_path_split)
	{
		create_cmd_path(&len, current_path_split, command, &exec_path);
		if (access(exec_path, X_OK) == 0)
		{
			execute_command_from_path(exec_path, path_split, command, data);
			return ;
		}
		free(exec_path);
		current_path_split++;
	}
	return (handle_cmd_not_found(path_split, command));
}

int	arg_helper(t_token *cur_token, t_com *current_cmd, int *arg_count)
{
	int	i;

	if (cur_token->type == TOKEN_SQUOTES)
	{
		printf("token thinks its squote\n");
		handle_squotes(current_cmd, *arg_count);
	}
	if (cur_token->type == TOKEN_DQUOTES)
		current_cmd->d_quote = 1;
	if (current_cmd->argv)
	{
		i = 0;
		while (current_cmd->argv[i])
		{
			free(current_cmd->argv[i]);
			current_cmd->argv[i] = NULL;
			i++;
		}
		free(current_cmd->argv);
		current_cmd->argv = NULL;
	}
	return (0);
}

int	temp_arg_creator(t_com *current_cmd, char **temp_argv, int *arg_count)
{
	int	i;

	i = 0;
	while (i < (*arg_count) - 1)
	{
		if (current_cmd->argv && current_cmd->argv[0])
			temp_argv[i] = ft_strdup(current_cmd->argv[i]);
		else
			temp_argv[i] = NULL;
		i++;
	}
	return (0);
}
