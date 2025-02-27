/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iduric <iduric@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:02:20 by lfaria-m          #+#    #+#             */
/*   Updated: 2025/02/27 00:56:15 by iduric           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_absolute(t_com *command, t_data *data)
{
	char	*com;
	char	**args;

	com = command->argv[0];
	args = command->argv;
	if (!access(com, X_OK))
		execve(com, args, env_list_to_envp(data->envp));
}

void	store_exit_status(int status)
{
	if (WIFEXITED(status))
	{
		g_exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

void	handle_child(t_com *cmd, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->delim)
	{
		if (handle_redirect_heredoc(cmd) != 0)
			exit(130);
	}
	if (cmd->output_file)
		handle_redirect_out(cmd);
	if (cmd->argv[0][0] == '/')
		handle_absolute(cmd, data);
	call_child_action(*cmd, data);
}

void	if_sigquit_sigint(int status)
{
	if (WTERMSIG(status) == SIGQUIT)
	{
		ft_putstr_fd("Quit\n", 2);
		g_exit_status = 131;
	}
	else if (WTERMSIG(status) == SIGINT)
	{
		ft_putchar_fd('\n', 2);
		g_exit_status = 130;
	}
}

void	execute_process(t_com *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	if (!cmd->is_builtin)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		pid = fork();
		if (pid == 0)
			handle_child(cmd, data);
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			if (WIFSIGNALED(status))
				if_sigquit_sigint(status);
			else
				store_exit_status(status);
			if (isatty(STDIN_FILENO))
				signal_handler_interactive();
		}
	}
	else
		execute_builtin_command(cmd, data);
}
