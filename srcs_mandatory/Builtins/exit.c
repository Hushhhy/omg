/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:12:57 by acarpent          #+#    #+#             */
/*   Updated: 2024/09/26 15:12:57 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_cmdline *cmdline)
{
	int	exit_stat;

	exit_stat = 0;
	if (!cmdline->cmd->cmds[1])
	{
		printf("exit\n");
		exit(exit_stat);
	}
	if (!is_num(cmdline->cmd->cmds[1]))
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit :", 2);
		ft_putstr_fd(cmdline->cmd->cmds[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	else if (!is_num(cmdline->cmd->cmds[1]) && !cmdline->cmd->cmds[2])
		ft_putstr_fd("exit\n", 2);
	if (cmdline->cmd->cmds[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(1);
	}
	exit_stat = ft_atoi(cmdline->cmd->cmds[1]);
	exit(exit_stat);
}
