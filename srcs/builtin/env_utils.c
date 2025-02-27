/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:36:28 by iduric            #+#    #+#             */
/*   Updated: 2025/02/26 23:52:33 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**env_list_to_envp(t_list *env_list)
{
	int		count;
	t_list	*tmp;
	char	**envp;
	size_t	len;
	int		i;

	count = 0;
	tmp = env_list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	tmp = env_list;
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
		ft_strlcpy(envp[i], tmp->name, len);
		ft_strlcat(envp[i], "=", len);
		ft_strlcat(envp[i], tmp->value, len);
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
