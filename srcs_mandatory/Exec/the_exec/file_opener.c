/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_opener.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:28:01 by codespace         #+#    #+#             */
/*   Updated: 2024/10/29 14:36:24 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_file_opener(t_redirs *current, int *last_fd_in)
{
	_maybe_fd_closing(*last_fd_in);
	if (access(current->infile, F_OK) != 0)
	{
		perror(current->infile);
		exit(1);
	}
	*last_fd_in = open(current->infile, O_RDONLY);
	if (*last_fd_in < 0)
	{
		perror(current->infile);
		exit(1);
	}
	redirect_and_close(*last_fd_in, STDIN_FILENO);
}

void	outfile_opener(t_redirs *current, int *last_fd_out)
{
	_maybe_fd_closing(*last_fd_out);
	if (current->out_flag == true)
		*last_fd_out = open(current->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (current->out_app == true)
		*last_fd_out = open(current->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*last_fd_out < 0)
	{
		perror(current->outfile);
		exit(1);
	}
	redirect_and_close(*last_fd_out, STDOUT_FILENO);
}

void	ft_open_files(t_ms *ms)
{
	t_redirs	*current;
	int			last_fd_out;
	int			last_fd_in;

	last_fd_in = -1;
	last_fd_out = -1;
	current = ms->cmdlines->cmd->redirs;
	while (current)
	{
		if (current->out_flag == true || current->out_app == true)
			outfile_opener(current, &last_fd_out);
		else if (current->in_flag == true || current->hd_flag == true)
			in_file_opener(current, &last_fd_in);
		current = current->next;
	}
}
