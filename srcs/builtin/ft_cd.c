/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:18:42 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/27 14:10:36 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_home(t_data *data)
{
	t_list	*env_temp;

	env_temp = data->envp;
	while (env_temp)
	{
		if (!ft_strncmp("HOME", env_temp->name, ft_strlen("HOME")))
			return (env_temp->value);
		env_temp = env_temp->next;
	}
	env_temp = data->local_env;
	while (env_temp)
	{
		if (!ft_strncmp("HOME", env_temp->name, ft_strlen("HOME")))
			return (env_temp->value);
		env_temp = env_temp->next;
	}
	return (NULL);
}

int	ft_cd(t_com command, t_data *data)
{
	char	*home_path;

	if (!command.argv[1])
	{
		home_path = find_home(data);
		if (!home_path)
		{
			chdir("/home");
			return (1);
		}
		if (!chdir(home_path))
			;
		else
			chdir("/home");
	}
	else if (command.argv[1])
	{
		if (chdir(command.argv[1]))
		{
			perror("not a valid directory");
			return (1);
		}
	}
	return (0);
}
