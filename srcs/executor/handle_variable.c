/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:59:13 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/27 14:16:45 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	expand_status(t_com *com, int j)
{
	if (!ft_strncmp(com->argv[j], "$?", 2))
	{
		free(com->argv[j]);
		com->argv[j] = NULL;
		com->argv[j] = ft_itoa(g_exit_status);
		return (1);
	}
	return (0);
}

void	handle_expand_status(t_list *vars, t_com *com, int j, char *rest_of_str)
{
	char	*new_value;

	while (vars)
	{
		if (!ft_memcmp(com->argv[j] + 1, vars->name, ft_strlen(com->argv[j]
					+ 1)))
		{
			new_value = ft_strjoin(vars->value, rest_of_str);
			free(com->argv[j]);
			com->argv[j] = new_value;
			free(rest_of_str);
			return ;
		}
		vars = vars->next;
	}
	free(com->argv[j]);
	com->argv[j] = ft_strjoin("", rest_of_str);
}

void	expand_variable(t_com *com, int j, t_list *vars)
{
	char	*new_value;
	char	*rest_of_str;
	int		var_len;

	var_len = 1;
	while (com->argv[j][var_len] && !isspace(com->argv[j][var_len]))
		var_len++;
	rest_of_str = ft_strdup(com->argv[j] + var_len);
	com->argv[j][var_len] = '\0';
	if (getenv(com->argv[j] + 1))
	{
		new_value = ft_strjoin(getenv(com->argv[j] + 1), rest_of_str);
		free(com->argv[j]);
		com->argv[j] = new_value;
	}
	else if (!expand_status(com, j))
		handle_expand_status(vars, com, j, rest_of_str);
	free(rest_of_str);
}

void	command_has_variable(t_com *com, t_list *local_env)
{
	int	j;

	j = 0;
	while (com->argv[j])
	{
		if (com->argv[j] && ft_strchr(com->argv[j], '$'))
		{
			if (!check_if_quotes(com, j))
				expand_variable(com, j, local_env);
		}
		j++;
	}
}
