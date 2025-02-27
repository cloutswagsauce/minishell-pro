/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:57:09 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/26 21:54:47 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	del_node_contents(void *vars)
{
	t_list	*node;

	node = (t_list *)vars;
	if (node->name)
		free(node->name);
	if (node->value)
		free(node->value);
}

int	check_local(t_com *com, t_list **vars)
{
	t_list	*temp;
	t_list	*prev;

	temp = *vars;
	prev = NULL;
	printf("the arg is %s\n", com->argv[1]);
	while (temp)
	{
		if (!ft_strncmp(com->argv[1], temp->name, ft_strlen(temp->name)))
		{
			printf("we found variable name: %s\n", temp->name);
			if (prev)
				prev->next = temp->next;
			else
				*vars = temp->next;
			ft_lstdelone(temp, del_node_contents);
			break ;
		}
		prev = temp;
		temp = temp->next;
	}
	return (0);
}

int	check_env(t_com *com, t_list **envp)
{
	t_list	*temp;
	t_list	*prev;

	temp = *envp;
	prev = NULL;
	while (temp)
	{
		if (!ft_strncmp(temp->name, com->argv[1], ft_strlen(temp->name)))
		{
			if (prev)
				prev->next = temp->next;
			else
				*envp = temp->next;
			ft_lstdelone(temp, del_node_contents);
			return (1);
		}
		prev = temp;
		temp = temp->next;
	}
	return (0);
}

int	ft_unset(t_com *com, t_data *data)
{
	if (!com->argv[1])
		return (0);
	if (check_env(com, &data->envp))
		return (0);
	if (check_local(com, &data->local_env))
		return (0);
	return (0);
}
