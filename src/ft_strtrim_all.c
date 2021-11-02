/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_all.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 13:33:57 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/02 20:00:46 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	malloc_len(char const *s1, char const *set)
{
	int	count;
	int	i;
	int	dquote;
	int	dquote1;
	int	squote;
	int	squote1;

	i = 0;
	count = 0;
	dquote = 0;
	dquote1 = 0;
	squote = 0;
	squote1 = 0;
	while (s1 && s1[i])
	{
		squote = (squote + (!dquote && s1[i] == '\'')) % 2;
		squote1 = (squote1 + (s1[i] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[i] == '\"')) % 2;
		dquote1 = (dquote1 + (s1[i] == '\"')) % 2;
		if ((s1[i] == '\"' && !squote) || (s1[i] == '\'' && !dquote))
			count++;
		i++;
	}
	if (squote || dquote)
		return (-1);
	return (count);
}

char	*ft_strtrim_all(char const *s1)
{
	int		count;
	int		i;
	int		j;
	char	*trimmed;
	int	dquote;
	int	squote;

	j = -1;
	dquote = 0;
	squote = 0;
	if (!s1)
		return (0);
	i = 0;
	count = malloc_len(s1, "\'\"");
	if (count == -1)
		return (NULL);
	trimmed = malloc(sizeof(char) * (ft_strlen(s1) - count + 1));
	if (!trimmed)
		return (NULL);
	while (s1[i])
	{
		squote = (squote + (!dquote && s1[i] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[i] == '\"')) % 2;
		if ((s1[i] != '\"' || squote) && (s1[i] != '\'' || dquote) && ++j >= 0)
			trimmed[j] = s1[i];
		i++;
	}
	trimmed[++j] = '\0';
	return (trimmed);
}
