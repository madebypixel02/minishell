/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/12 00:13:13 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	**expand_matrix(char ***args, int quotes[2], t_prompt *prompt)
{
	char	**aux;
	int		i;

	i = -1;
	aux = ft_calloc(sizeof(char *), (ft_matrixlen(*args) + 1));
	while (aux && ++i >= 0 && args[0][i])
	{
		aux[i] = expand_vars(ft_strdup(args[0][i]), -1, quotes, prompt);
		aux[i] = expand_path(aux[i], -1, quotes, \
			mini_getenv("HOME", prompt->envp, 4));
		if (!aux[i])
		{
			mini_perror(QUOTE, NULL);
			ft_free_matrix(args);
			ft_free_matrix(&aux);
			return (NULL);
		}
	}
	return (aux);
}

static char	**split_all(char **args)
{
	char	**subsplit;
	int		i;

	i = -1;
	while (args[++i])
	{
		subsplit = ft_subsplit(args[i], "\"\'<|>");
		ft_matrix_replace_in(&args, subsplit, i);
		i += ft_matrixlen(subsplit) - 1;
		ft_free_matrix(&subsplit);
	}
	return (args);
}

t_list	*parse_args(char **args, t_prompt *prompt)
{
	t_list	*cmds;
	int		quotes[2];

	cmds = NULL;
	args = expand_matrix(&args, quotes, prompt);
	args = split_all(args);
	cmds = fill_nodes(args);
	if (!args)
		return (NULL);
	ft_free_matrix(&args);
	return (cmds);
}
