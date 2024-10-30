/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_opener.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:28:01 by codespace         #+#    #+#             */
/*   Updated: 2024/10/30 14:16:48 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_file_opener(t_ms *ms, int *last_fd_in)
{
	_maybe_fd_closing(*last_fd_in);
	if (access(ms->cmdlines->cmd->redirs->infile, F_OK) != 0)
	{
		perror(ms->cmdlines->cmd->redirs->infile);
		clean_child(ms);
		exit(1);
	}
	*last_fd_in = open(ms->cmdlines->cmd->redirs->infile, O_RDONLY);
	if (*last_fd_in < 0)
	{
		perror(ms->cmdlines->cmd->redirs->infile);
		clean_child(ms);
		exit(1);
	}
	redirect_and_close(*last_fd_in, STDIN_FILENO);
}

void	outfile_opener(t_ms *ms, int *last_fd_out)
{
	_maybe_fd_closing(*last_fd_out);
	if (ms->cmdlines->cmd->redirs->out_flag == true)
		*last_fd_out = open(ms->cmdlines->cmd->redirs->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ms->cmdlines->cmd->redirs->out_app == true)
		*last_fd_out = open(ms->cmdlines->cmd->redirs->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*last_fd_out < 0)
	{
		perror(ms->cmdlines->cmd->redirs->outfile);
		clean_child(ms);
		exit(1);
	}
	redirect_and_close(*last_fd_out, STDOUT_FILENO);
}

void	ft_open_files(t_ms *ms)
{
	int			last_fd_out;
	int			last_fd_in;

	last_fd_in = -1;
	last_fd_out = -1;
	while (ms->cmdlines->cmd->redirs)
	{
		if (ms->cmdlines->cmd->redirs->out_flag == true || ms->cmdlines->cmd->redirs->out_app == true)
			outfile_opener(ms, &last_fd_out);
		else if (ms->cmdlines->cmd->redirs->in_flag == true || ms->cmdlines->cmd->redirs->hd_flag == true)
			in_file_opener(ms, &last_fd_in);
		ms->cmdlines->cmd->redirs = ms->cmdlines->cmd->redirs->next;
	}
}
