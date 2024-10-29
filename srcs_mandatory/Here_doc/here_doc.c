/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:05:06 by codespace         #+#    #+#             */
/*   Updated: 2024/10/29 19:47:26 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_tmp_file(t_ms *ms)
{
	char	*filename;

	filename = create_filename();
	if (filename == NULL)
		return (NULL);
	if (open_file(ms, filename) == -1)
	{
		free(filename);
		return (NULL);
	}
	return (filename);
}

void	read_and_write_lines(t_ms *ms, char *filename, char *limiter, int saved_stdin)
{
	char	*line;

	signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			handle_null_line(ms, filename, limiter);
			close(ms->here_doc_fd);
			if (g_var == CTRL_C)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close_all_fds();
				return;
			}
			return;
		}
		if (line && check_line_against_limiter(line, limiter))
			break;
		write_line_to_file(ms, line);
		free(line);
	}
}

void	handle_child_process(t_ms *ms, t_token *limiter, char *tmp_file)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		int saved_stdin = dup(STDIN_FILENO);
		ms->here_doc_fd = open(tmp_file, O_WRONLY | O_APPEND);
		if (ms->here_doc_fd == -1)
		{
			perror("Error opening temp file in child");
			exit(EXIT_FAILURE);
		}
		read_and_write_lines(ms, tmp_file, limiter->value, saved_stdin);
		close(ms->here_doc_fd);
		close(saved_stdin);
		exit(0);
	}
	else
	{
		signal(SIGINT, sigint_heredoc);
		waitpid(pid, &status, 0);
	}
}

void	process_current_token(t_ms *ms, t_token *current)
{
	t_token	*limiter;
	char	*tmp_file;

	if (current->type == HERE_DOC)
	{
		limiter = current->next;
		if (limiter != NULL && limiter->type == LIMITER)
		{
			tmp_file = create_tmp_file(ms);
			if (tmp_file != NULL)
			{
				handle_child_process(ms, limiter, tmp_file);
				free(limiter->value);
				limiter->value = tmp_file;
				limiter->type = INFILE;
			}
		}
	}
}

void	handle_here_doc(t_ms *ms, t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	here_doc_count(current);
	if (g_var == CTRL_C)
		g_var = 0;
	while (current != NULL)
	{
		if (g_var == CTRL_C)
		{
			ms->v_return = CTRL_C;
			break;
		}
		process_current_token(ms, current);
		current = current->next;
	}
}
