/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:28:36 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/28 17:51:45 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	ft_count_words(const char *s, char c)
{
	int		count;
	int		quote;
	int		i;
	char	type;

	i = 0;
	count = 0;
	quote = 0;
	type = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			count++;
			while ((s[i] != c || quote) && s[i] != '\0')
			{
				if (!type && (s[i] == '\"' || s[i] == '\''))
					type = s[i];
				quote = (quote + (s[i] == type)) % 2;
				i++;
			}
			if (quote)
				return (-1);
		}
		else
			i++;
	}
	return (count);
}

static char	**ft_fill_array(char **aux, char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		k;
	size_t	s_len;
	int		quote;
	char	type;

	i = 0;
	k = 0;
	quote = 0;
	type = 0;
	s_len = ft_strlen(s);
	while (s[i])
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		j = i;
		while ((s[i] != c || quote) && s[i] != '\0')
		{	
			if (!type && (s[i] == '\"' || s[i] == '\''))
				type = s[i];
			quote = (quote + (s[i] == type)) % 2;
			i++;
		}
		if (j >= s_len)
			aux[k++] = "\0";
		else
			aux[k++] = ft_substr(s, j, i - j);
		aux[k - 1] = ft_strtrim(aux[k - 1], "\"\'");
	}
	return (aux);
}

char	**ft_cmdtrim(char const *s, char c)
{
	char	**aux;
	int		nwords;

	if (!s)
		return (NULL);
	nwords = ft_count_words(s, c);
	if (nwords == -1)
		return (NULL);
	aux = malloc((nwords + 1) * sizeof(char *));
	if (aux == NULL)
		return (NULL);
	aux = ft_fill_array(aux, s, c);
	aux[nwords] = NULL;
	return (aux);
}
