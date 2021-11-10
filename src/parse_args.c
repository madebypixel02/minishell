/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/10 12:52:20 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_mini	*mini_init(void)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->full_cmd = NULL;
	mini->full_path = NULL;
	mini->infile = STDIN_FILENO;
	mini->outfile = STDOUT_FILENO;
	return (mini);
}

static char	**expand_matrix(char ***args, int quotes[2], t_prompt *prompt)
{
	char	**aux;
	char	*str;
	int		i;

	i = -1;
	aux = ft_calloc(sizeof(char *), (ft_matrixlen(*args) + 1));
	while (aux && ++i >= 0 && args[0][i])
	{
		str = expand_vars(ft_strdup(args[0][i]), -1, quotes, prompt);
		str = expand_path(str, -1, quotes, \
			mini_getenv("HOME", prompt->envp, 4));
		aux[i] = ft_strtrim_all(str, 0, 0);
		free(str);
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

t_list	*parse_args(char **args, t_prompt *prompt)
{
	t_list	*cmds;
	t_mini	*node;
	int		quotes[2];

	cmds = NULL;
	args = expand_matrix(&args, quotes, prompt);
	if (!args)
		return (NULL);
	node = mini_init();
	if (!node)
		return (NULL);
	fill_node(args, node);
	if (!(node))
	{
		ft_lstclear(&cmds, free);
		return (NULL);
	}
	ft_lstadd_back(&cmds, ft_lstnew(node));
	ft_free_matrix(&args);
	return (cmds);
}
