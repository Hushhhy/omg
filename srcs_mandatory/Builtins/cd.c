/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:12:43 by acarpent          #+#    #+#             */
/*   Updated: 2024/09/26 15:12:43 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	changedir(char *path)
{
	char		*new;

	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	else
	{
		new = getcwd(NULL, 0);
		setenv("PWD", new, 1);
		free(new);
	}
	return (0);
}

int	ft_cd(t_cmdline *cmdline)
{
	if (!cmdline->cmd->cmds[1])
		changedir(getenv("HOME"));
	else if (cmdline->cmd->cmds[1])
	{
		if (cmdline->cmd->cmds[2])
		{
			ft_putstr_fd("minishel: cd: too many arguments\n", 2);
			return (1);
		}
		else if (changedir(cmdline->cmd->cmds[1]))
			return (1);
	}
	else
	{
		ft_putstr_fd("minishell: cd: usage: cd <path>\n", 2);
		return (1);
	}
	return (0);
}
