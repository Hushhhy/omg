/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrellie <pgrellie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:08:24 by pgrellie          #+#    #+#             */
/*   Updated: 2024/10/28 18:42:13 by pgrellie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*e_tracker(char *s)
{
	int		j;
	int		i;
	char	*tmp;

	if (!s)
		return (NULL);
	tmp = NULL;
	i = 0;
	if (s[i] == '$')
	{
		i++;
		j = i;
		while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
			i++;
		tmp = ft_substr(s, j, i - j);
		if (!tmp)
			return (NULL);
	}
	return (tmp);
}

void	maybe_del_node(t_token *tok, t_env *env)
{
	char	*e_name;
	t_env	*current;

	if (tok->value[0] == '$' && !tok->value[1])
		return ;
	if ((tok->value[0] == '$' && tok->value[1] == '?')
		|| (tok->value[0] == '$' && tok->value[1] == '$'))
		return ;
	e_name = e_tracker(tok->value);
	if (!e_name)
		return ;
	current = find_env(env, e_name);
	free(e_name);
	if (!current)
	{
		tok->to_del = true;
		return ;
	}
	else if (current && !current->value)
		tok->to_del = true;
	else if (current->value && ft_strcmp(current->value, "\"\"") == 0)
		tok->to_del = true;
	else
		return ;
}

void	double_dollar(char *fs, int *x, int *y)
{
	fs[(*y)++] = '$';
	fs[(*y)++] = '$';
	*x += 2;
}

void	dollar_bruh(char *fs, int *x, int *y, int v_return)
{
	char	*v_return_str;
	int		i;

	i = 0;
	v_return_str = ft_itoa(v_return);
	if (!v_return_str)
		return ;
	while (v_return_str[i])
	{
		fs[(*y)++] = v_return_str[i++];
	}
	free(v_return_str);
	*x += 2;
}

void	expand_variable(t_token *tok, t_env *env, t_var *v, char *fs)
{
	char	*var_name;
	t_env	*ev;
	char	*var_value;
	int		i;

	i = 0;
	var_name = tracker(tok->value, &v->x);
	if (!var_name)
		return ;
	ev = find_node(var_name, env);
	free(var_name);
	if (!ev || !ev->value)
		return ;
	var_value = ev->value;
	if (ev->value == NULL || ft_strcmp(ev->value, "\"\"") == 0)
		tok->to_del = true;
	while (var_value[i])
	{
		fs[v->y++] = var_value[i++];
	}
}

// char	*expand_env_value(char *fs, char *env_value, int *x, char *tok_value)
// {
// 	char	*tmp;

// 	if (!fs || !env_value || !x || !tok_value)
// 		return (NULL);
// 	tmp = ft_strjoin(fs, env_value);
// 	if (!tmp)
// 		return (free(fs), NULL);
// 	return (tmp);
// }

// char	*expand_variable(t_token *tok, t_env *env, int *x, char *fs)
// {
// 	t_env	*ev;
// 	char	*tmp;

// 	tmp = tracker(tok->value, x);
// 	if (!tmp)
// 		return (free(fs), NULL);
// 	ev = find_node(tmp, env);
// 	free(tmp);
// 	if (!ev)
// 		return (fs);
// 	return (expand_env_value(fs, ev->value, x, tok->value));
// }

/********************************************************************/

// Entre double quote -> remplacement

// Entre single quote -> pas de remplacement

// $EXPAND doit etre suivi d'un espace pour etre remplace

// $$ -> NULL

// Malloc la nouvelle chaine moins les quotes

// $? -> renvoi la valeur de retour (A gerer a part
// 			des variables d'environnement)avec EXIT_STATUS
/********************************************************************/

// char	*dr_kron(t_token *tok, t_env *env, int v_return)
// {
// 	t_env	*ev;
// 	char	*fs;
// 	char	*tmp;
// 	int		x;
// 	int		y;

// 	if (!tok || !env || !tok->value || !env->value)
// 		return (NULL);
// 	x = 0;
// 	y = 0;
// 	fs = ft_calloc(1, sizeof(char) * ft_strlen(tok->value) + 1);
// 	if (!fs)
// 		return (NULL);
// 	printf("Debug: fs after calloc: %s\n", fs);
// 	while (tok->value[x])
// 	{
// 		if (tok->value[x] == '$' && tok->value[x + 1] == '?')
// 		{
// 			fs = dollar_bruh(fs, &x, v_return);
// 			if (!fs)
// 				return (NULL);
// 			y = ft_strlen(fs);
// 		}
// 		if (tok->value[x] == '$' && expandable(tok->value, &x))
// 		{
// 			tmp = tracker(tok->value, &x);
// 			if (!tmp)
// 				return (free(fs), NULL);
// 			printf("Debug: tracker returned %s\n", tmp);
// 			ev = find_node(tmp, env);
// 			free(tmp);
// 			if (!ev)
// 				continue ;
// 			printf("name: \033[0;36m %-20s \033[0m |\t
// 				value: \033[0;35m %-18s \033[0m \n%d",
// 				ev->name, ev->value, ev->equal_sign);
// 			printf("--------------------------------------------------\n");
// 			fs = expand_env_value(fs, ev->value, &x, tok->value);
// 			printf("Debug: fs after expand_env_value: %s\n", fs);
// 			y = ft_strlen(fs);
// 		}
// 		else
// 		{
// 			fs[y++] = tok->value[x++];
// 			printf("Debug: y incremented to: %d\n", y);
// 		}
// 	}
// 	fs[y] = '\0';
// 	printf("Debug: y incremented to: %d\n", y);
// 	printf("Debug: final fs: %s\n", fs);
// 	return (transformer(tok, fs));
// }
