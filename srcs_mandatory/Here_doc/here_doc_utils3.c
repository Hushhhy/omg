/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:07:42 by acarpent          #+#    #+#             */
/*   Updated: 2024/10/30 16:35:18 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_std(int saved_stdin)
{
	dup2(saved_stdin, STDIN_FILENO);
	close_all_fds();
	return ;
}

void	zero_pid_handle(int saved_stdin, char *tmp_file,
	t_token *limiter, t_ms *ms)
{
	saved_stdin = dup(STDIN_FILENO);
	ms->here_doc_fd = open(tmp_file, O_WRONLY | O_APPEND);
	if (ms->here_doc_fd == -1)
	{
		perror("Error opening temp file in child");
		clean_hd_child(ms);
		delete_tmp_files();
		exit(EXIT_FAILURE);
	}
	read_and_write_lines(ms, tmp_file, limiter->value, saved_stdin);
	clean_hd_child(ms);
	exit(0);
}

void	clean_hd_child(t_ms *ms)
{
	close_all_fds();
	free_tokens(&ms->tokens);
	free_env(&ms->env);
	free(ms->prompt);
	free(ms);
}

void clear_program(t_ms *ms)
{
	close_all_fds();
	if (ms->cmdlines)
		clear_cmdlines(&ms->cmdlines);
	if (ms->envi)
		ft_free_tab(ms->envi);
	if (ms->tokens)
		free_tokens(&ms->tokens);
	if (ms->env)
		free_env(&ms->env);
	if (ms->prompt)
		free(ms->prompt);
	if (ms)
		free(ms);
}
