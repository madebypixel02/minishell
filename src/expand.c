/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:17:55 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/12 19:55:30 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**expand_path(char **str, int i, int quotes[2], char *var)
{
	char	*aux[2];
	int		pos;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i][0] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i][0] == '\"')) % 2;
		if (!quotes[0] && !quotes[1] && ft_strchr(str[i], '~'))
		{
			pos = ft_strchr_i(str[i], '~');
			aux[1] = ft_substr(str[i], 0, pos);
			aux[0] = ft_strjoin(aux[1], var);
			free(aux[1]);
			aux[1] = ft_substr(str[i], pos + 1, ft_strlen(str[i]));
			free(str[i]);
			str[i] = ft_strjoin(aux[0], aux[1]);
			free(aux[1]);
			free(aux[0]);
			return (expand_path(str, --i, quotes, var));
		}
	}
	free(var);
	return (str);
}

static char	**get_substr_var(char **str, int i, t_prompt *prompt)
{
	char	*aux;
	int		pos;
	char	*path;
	char	*var;
	int		pos2;

	pos = ft_strchars_i(str[i], "$");
	if (pos == -1)
		pos = ft_strlen(str[i]) - 1;
	pos2 = ft_strchars_i(&str[i][pos + 1], "$ ");
	if (pos2 == -1)
		pos2 = ft_strlen(&str[i][pos + 1]);
	aux = ft_substr(str[i], 0, pos);
	var = mini_getenv(&str[i][pos + 1], prompt->envp, pos2);
	path = ft_strjoin(aux, var);
	free(aux);
	aux = ft_strjoin(path, &str[i][pos2 + pos + 1]);
	free(var);
	free(str[i]);
	str[i] = aux;
	free(path);
	return (str);
}

char	**expand_vars(char **str, int i, int quotes[2], t_prompt *prompt)
{
	int	pos;
	
	while (str && str[i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i][0] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i][0] == '\"')) % 2;
		pos = ft_strchr_i(str[i], '$');
		if (!quotes[0] && pos != -1 && ((quotes[1] && ft_strchars_i(str[i + 1], " \"")) || (!quotes[1] && !str[i][pos + 1] && (!str[i + 1] || str[i + 1][0] != ' '))))
		{
			return (expand_vars(get_substr_var(str, i, prompt), i, quotes, prompt));
		}
		i++;
	}
	return (str);
}
