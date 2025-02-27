/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfaria-m <lfaria-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:36:28 by iduric            #+#    #+#             */
/*   Updated: 2025/02/27 14:31:32 by lfaria-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_cats(char **envp, int i, t_list *tmp, int len)
{
	ft_strlcpy(envp[i], tmp->name, len);
	ft_strlcat(envp[i], "=", len);
	ft_strlcat(envp[i], tmp->value, len);
}

int	init_envp_list(t_list *tmp, t_list *env_list, char ***envp)
{
	int	count;

	count = 0;
	tmp = env_list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	*envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (1);
	tmp = env_list;
	return (0);
}

char	**env_list_to_envp(t_list *env_list)
{
	t_list	*tmp;
	char	**envp;
	size_t	len;
	int		i;

	tmp = NULL;
	init_envp_list(tmp, env_list, &envp);
	i = 0;
	while (tmp)
	{
		len = ft_strlen(tmp->name) + ft_strlen(tmp->value) + 2;
		envp[i] = malloc(len);
		if (!envp[i])
		{
			while (i > 0)
				free(envp[--i]);
			free(envp);
			return (NULL);
		}
		handle_cats(envp, i, tmp, len);
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
