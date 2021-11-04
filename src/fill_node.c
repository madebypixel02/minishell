/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/04 18:04:34 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	get_fd(int oldfd, char *path, int is_outfile, int append)
{
	int	fd;

	if (!path)
		return (-1);
	if (oldfd > 2)
		close(oldfd);
	//printf("Path: %s\n", path);
	if (is_outfile && append)
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (is_outfile && !append)
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (!is_outfile)
		fd = open(path, O_RDONLY);
	return (fd);
}

static	t_mini	*get_outfile(t_mini *node, char **args, char **arg, int ij[2])
{
	if (arg[ij[1]][0] == '>' && arg[ij[1] + 1] && arg[ij[1] + 1][0] == '>')
	{
		if (arg[ij[1] + 2])
			node->outfile = get_fd(node->outfile, &arg[ij[1] + 2][0], 1, 1);
		else
			node->outfile = get_fd(node->outfile, args[++ij[0]], 1, 1);
		ij[1]++;
	}
	else if (arg[ij[1]][0] == '>')
	{
		if (arg[ij[1] + 1])
			node->outfile = get_fd(node->outfile, &arg[ij[1] + 1][0], 1, 0);
		else
			node->outfile = get_fd(node->outfile, args[++ij[0]], 1, 0);
	}
	else if (arg[ij[1]][0] == '<')
	{
		if (arg[ij[1] + 1])
			node->infile = get_fd(node->infile, &arg[ij[1] + 1][0], 0, 0);
		else
			node->infile = get_fd(node->infile, args[++ij[0]], 0, 0);
	}
	else if (!node->cmd)
	{
		node->cmd = ft_strdup(arg[ij[1]]);
		node->full_cmd = ft_extend_matrix(node->full_cmd, node->cmd);
	}
	else
		node->full_cmd = ft_extend_matrix(node->full_cmd, ft_strdup(arg[ij[1]]));
	return (node);
}

static	t_mini	*check_arg(t_mini *node, char **args, char **arg, int ij[2])
{
	get_outfile(node, args, arg, ij);
	return (node);
}

t_mini	*fill_node(char **args, t_mini	*node)
{
	int		ij[2];
	char	**arg;

	ij[0] = 0;
	while (args[ij[0]])
	{
		ij[1] = -1;
		arg = ft_subsplit(args[ij[0]], "<>");
		printf("------------------\n");
		ft_putmatrix_fd(arg, 1);
		if (!arg)
			return (NULL);
		while (arg[++ij[1]])
			check_arg(node, args, arg, ij);
		ft_free_matrix(&arg);
		ij[0] += (args[ij[0]] != NULL);
	}
	return (node);
}
