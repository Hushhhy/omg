/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_programm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*    <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:51:23 by pgrellie          #+#    #+#             */
/*   Updated: 2024/10/28 01:12:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_var = 0;

int	handle_ctrld(t_ms *ms)
{
	if (!ms->prompt)
	{
		printf("exit\n");
		return (1);
	}
	return (0);
}

char	*prompt(t_ms *ms)
{
	char	*input;

	input = readline(PROMPT_LINE);
	if (input == NULL)
		return (NULL);
	if (input[0] == '\0' || !ft_strcmp(input, ":")
		|| !ft_strcmp(input, "!") || !ft_strcmp(input, "#"))
	{
		if (!ft_strcmp(input, ":") || !ft_strcmp(input, "#"))
			ms->v_return = 0;
		if (!ft_strcmp(input, "!"))
			ms->v_return = 1;
		free(input);
		return (ft_strdup(""));
	}
	add_history(input);
	return (input);
}

t_ms	*init_ms(void)
{
	t_ms	*ms;

	ms = malloc(sizeof(t_ms));
	if (ms == NULL)
	{
		ft_putstr_fd("Error: couldn't allocate memory\n", 2);
		exit(1);
	}
	ms->prompt = NULL;
	ms->envi = NULL;
	ms->env = NULL;
	ms->tokens = NULL;
	ms->cmdlines = NULL;
	ms->v_return = 0;
	ms->c_count = 0;
	ms->pid = NULL;
	ms->previous_fd = -1;
	ms->pipefd[0] = -1;
	ms->pipefd[1] = -1;
	ms->here_doc_fd = -1;
	return (ms);
}

t_ms	*init_program(char **env)
{
	t_ms	*ms;

	ms = init_ms();
	ms->env = init_env(env);
	return (ms);
}

void	the_program(t_ms *ms)
{
	while (true)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint);
		ms->b = false;
		ms->prompt = prompt(ms);
		if (handle_ctrld(ms))
			break ;
		if (g_var != 0)
		{
			ms->v_return = g_var;
			g_var = 0;
		}
		if (full_check(ms) == false)
			continue ;
		handle_here_doc(ms, &ms->tokens);
		if (ms->v_return == CTRL_C)
		{
			ms->v_return = 0;
			g_var = 0;
			continue ;
		}
		if (ms->tokens)
		{
			ms->v_return = executor(ms);
			free_tokens(&ms->tokens);
		}
		free(ms->prompt);
		delete_tmp_files();
	}
	free_env(&ms->env);
	free(ms->prompt);
}
