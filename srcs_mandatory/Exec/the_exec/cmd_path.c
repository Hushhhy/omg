/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 00:32:36 by codespace         #+#    #+#             */
/*   Updated: 2024/10/29 15:05:14 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_directory(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		ft_putstr_fd("MYSHELL : Is a directory\n", 2);
		free(path);
		exit(126);
	}
}

void	check_executable(char *path)
{
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("MYSHELL : Permission denied\n", 2);
		free(path);
		exit(126);
	}
}

char	*handle_direct_path(char *cmd)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		path = ft_strdup(cmd);
		if (access(path, F_OK) == 0)
		{
			check_directory(path);
			check_executable(path);
		}
		return (path);
	}
	return (NULL);
}

char	*search_in_path(char **envi, char *cmd)
{
	int		x;
	char	*path;
	char	**pathsss;

	x = 0;
	while (envi[x])
	{
		if (ft_strncmp(envi[x], "PATH=", 5) == 0)
		{
			path = ft_strdup(envi[x] + 5);
			pathsss = ft_split(path, ':');
			free(path);
			path = env_for_exec(pathsss, cmd);
			if (path)
				return (path);
		}
		x++;
	}
	return (NULL);
}

char	*cmd_path(char **envi, char *cmd)
{
	char	*path;

	if (!cmd)
		return (NULL);
	path = handle_direct_path(cmd);
	if (path)
		return (path);
	else
		return (search_in_path(envi, cmd));
}
