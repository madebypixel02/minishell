/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/09 11:58:57 by mbueno-g         ###   ########.fr       */
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

static char	**expand_matrix(char ***args, int quotes[2])
{
	char	**aux;
	char	*str;
	int		i;
	int		ij[2];

	i = -1;
	aux = ft_calloc(sizeof(char *), (ft_matrixlen(*args) + 1));
	while (aux && ++i >= 0 && args[0][i])
	{
		str = expand_vars(ft_strdup(args[0][i]), ij, quotes);
		str = expand_path(ft_strdup(str), ij, quotes);
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

t_list	*parse_args(char **args)
{
	t_list	*cmds;
	t_mini	*node;
	int		quotes[2];

	cmds = NULL;
	args = expand_matrix(&args, quotes);
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

/*int	main(void)
{
	char	*str;
	char	**matrix1;
	t_list	*cmds;

	str = "cat << hola > out";
	matrix1 = ft_cmdtrim(str, " ");
	ft_putmatrix_fd(matrix1, 1);
	if (!matrix1)
		return (0);
	cmds = parse_args(matrix1);
	if (!cmds)
		return (0);
	//dup2(STDOUT_FILENO, 3);
	//close(STDOUT_FILENO);
	printf("\nStdin: %d\nStdout: %d\n\n", ((t_mini *)cmds->content)->infile, \
		((t_mini *)cmds->content)->outfile);
	ft_putmatrix_fd(((t_mini *)cmds->content)->full_cmd, 1);
	close(((t_mini *)cmds->content)->infile);
	close(((t_mini *)cmds->content)->outfile);
	ft_free_matrix(&((t_mini *)cmds->content)->full_cmd);
	ft_lstclear(&cmds, free);
}*/
