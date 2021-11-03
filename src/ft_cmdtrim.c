/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:28:36 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/03 10:02:08 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	ft_count_words(const char *s, char c, int counts[2])
{
	int		quote;
	char	type;

	quote = 0;
	type = 0;
	while (s[counts[0]] != '\0')
	{
		if (s[counts[0]] != c)
		{
			counts[1]++;
			while ((s[counts[0]] != c || quote) && s[counts[0]] != '\0')
			{
				if (!type && (s[counts[0]] == '\"' || s[counts[0]] == '\''))
					type = s[counts[0]];
				quote = (quote + (s[counts[0]] == type)) % 2;
				counts[0]++;
			}
			if (quote)
				return (-1);
		}
		else
			counts[0]++;
	}
	return (counts[1]);
}

static char	**ft_fill_array(char **aux, char const *s, char c, int i[3])
{
	int		s_len;
	int		quote;
	char	type;

	quote = 0;
	type = 0;
	s_len = ft_strlen(s);
	while (s[i[0]])
	{
		while (s[i[0]] == c && s[i[0]] != '\0')
			i[0]++;
		i[1] = i[0];
		while ((s[i[0]] != c || quote) && s[i[0]] != '\0')
		{	
			if (!type && (s[i[0]] == '\"' || s[i[0]] == '\''))
				type = s[i[0]];
			quote = (quote + (s[i[0]] == type)) % 2;
			i[0]++;
		}
		if (i[1] >= s_len)
			aux[i[2]++] = "\0";
		else
			aux[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	return (aux);
}

char	**ft_cmdtrim(char const *s, char c)
{
	char	**aux;
	int		nwords;
	int		i[3];
	int		counts[2];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	counts[0] = 0;
	counts[1] = 0;
	if (!s)
		return (NULL);
	nwords = ft_count_words(s, c, counts);
	if (nwords == -1)
		return (NULL);
	aux = malloc((nwords + 1) * sizeof(char *));
	if (aux == NULL)
		return (NULL);
	aux = ft_fill_array(aux, s, c, i);
	aux[nwords] = NULL;
	return (aux);
}
