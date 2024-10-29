/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 17:44:59 by pgrellie          #+#    #+#             */
/*   Updated: 2024/10/29 14:36:45 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*epure_tokens(t_ms *ms)
{
	t_token	*current;
	t_token	*temp;

	if (!ms || !ms->tokens)
		return (NULL);
	current = ms->tokens;
	while (current)
	{
		if (current->to_del)
		{
			temp = current->next;
			delete_token(&ms->tokens, current);
			current = temp;
		}
		else
		{
			current = current->next;
		}
	}
	return (ms->tokens);
}

bool	full_check(t_ms *ms)
{
	if (q_check(ms->prompt) == false || c_check(ms->prompt) == false
		|| shit_check_1(ms->prompt) == false)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `;'\n", 2);
		return (false);
	}
	else
	{
		ms->tokens = lexer(ms->prompt);
		expander(ms);
		epure_tokens(ms);
		if (_redirs_checker(ms) == false)
		{
			ft_putstr_fd(
				"MYSHELL: syntax error near unexpected token `newline'\n",
				2);
			ms->v_return = 2;
			return (false);
		}
		else
			return (true);
	}
}
