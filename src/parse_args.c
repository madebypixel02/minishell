/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/12 19:55:32 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	**expand_matrix(char **args, int quotes[2], t_prompt *prompt)
{
	char	**aux;

	aux = ft_calloc(sizeof(char *), (ft_matrixlen(args) + 1));
	aux = expand_vars(ft_dup_matrix(args), 0, quotes, prompt);
	aux = expand_path(aux, -1, quotes, mini_getenv("HOME", prompt->envp, 4));
	if (!aux)
	{
		mini_perror(QUOTE, NULL);
		ft_free_matrix(&args);
		ft_free_matrix(&aux);
		return (NULL);
	}
	ft_free_matrix(&args);
	return (aux);
}

/*static char	**split_all(char **args)
{
	char	**subsplit;
	int		i;

	i = -1;
	while (args && args[++i])
	{
		subsplit = ft_subsplit(args[i], "\"\'<|>");
		ft_matrix_replace_in(&args, subsplit, i);
		i += ft_matrixlen(subsplit) - 1;
		ft_free_matrix(&subsplit);
	}
	return (args);
}*/

t_list	*parse_args(char **args, t_prompt *prompt)
{
	t_list	*cmds;
	int		quotes[2];

	cmds = NULL;
	quotes[0] = 0;
	quotes[1] = 0;
	args = expand_matrix(args, quotes, prompt);
	//args = split_all(args);
	if (!args)
		return (NULL);
	cmds = fill_nodes(args);
	//ft_putmatrix_fd(((t_mini *)cmds->content)->full_cmd, 1);
	//printf("----------------------------\n");
	ft_free_matrix(&args);
	return (cmds);
}
