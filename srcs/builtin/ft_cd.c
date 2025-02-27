/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:18:42 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/26 21:35:56 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_cd(t_com command)
{
	if (!command.argv[1])
		chdir("/");
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
