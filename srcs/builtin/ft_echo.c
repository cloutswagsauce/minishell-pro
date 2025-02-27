/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 17:45:16 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/26 21:36:06 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_echo(t_com command)
{
	int	i;

	if (command.argv[1] && !ft_strncmp(command.argv[1], "-n", 2))
	{
		i = 2;
		while (command.argv[i])
		{
			ft_putstr_fd(command.argv[i], 1);
			write(1, " ", 1);
			i++;
		}
		return (0);
	}
	else
	{
		i = 1;
		while (command.argv[i])
		{
			ft_putstr_fd(command.argv[i], 1);
			write(1, " ", 1);
			i++;
		}
		write(1, "\n", 1);
		return (0);
	}
}
