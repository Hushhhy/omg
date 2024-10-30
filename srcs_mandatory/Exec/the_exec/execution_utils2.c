/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarpent <acarpent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:02:34 by acarpent          #+#    #+#             */
/*   Updated: 2024/10/30 16:46:08 by acarpent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_and_free(t_ms *ms)
{
	clear_cmdlines(&ms->cmdlines);
	ft_free_tab(ms->envi);
	free(ms->pid);
	close(ms->pipefd[0]);
	close(ms->pipefd[1]);
}
