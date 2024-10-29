/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_in_child.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:38:24 by codespace         #+#    #+#             */
/*   Updated: 2024/10/29 18:20:08 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_for_exec(char **envi, char *cmd)
{
	int		x;
	char	*path;
	char	*tmp;

	x = 0;
	while (envi[x])
	{
		path = ft_strjoin(envi[x], "/");
		tmp = path;
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_free_tab(envi);
			return (path);
		}
		free(path);
		x++;
	}
	ft_free_tab(envi);
	return (NULL);
}

void	cmd_exec(t_ms *ms, char *cmd)
{
	if (cmd == NULL || ft_strlen(cmd) == 0)
	{
		ft_putstr_fd("MYSHELL: command not found\n", 2);
		ft_free_tab(ms->cmdlines->cmd->cmds);
		exit(127);
	}
	cmd = cmd_path(ms->envi, ms->cmdlines->cmd->cmds[0]);
	if (cmd)
	{
		execve(cmd, ms->cmdlines->cmd->cmds, ms->envi);
		ft_free_tab(ms->cmdlines->cmd->cmds);
		free(cmd);
		handle_exec_error();
	}
	else
	{
		ft_putstr_fd("MYSHELL : command not found\n", 2);
		ft_free_tab(ms->cmdlines->cmd->cmds);
		exit(127);
	}
}

void	child_process(t_ms *ms)
{
	redirector(ms);
	ft_open_files(ms);
	ms->v_return = builtins(ms);
	if (ms->b == true)
		exit(ms->v_return);
	if (ms->cmdlines->cmd->cmds[0])
		cmd_exec(ms, ms->cmdlines->cmd->cmds[0]);
	exit(127);
}
