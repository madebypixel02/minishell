/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/09 10:37:39 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_mini	*get_outfile1(t_mini *node, char **args, char **arg, int ij[2])
{
	char	**next;

	next = NULL;
	if (arg[ij[1] + 1])
		node->outfile = get_fd(node->outfile, &arg[++ij[1]][0], 1, 0);
	else
	{
		next = ft_subsplit(args[ij[0] + 1], "<>");
		ij[0] += ft_matrixlen(next) == 1;
		if (next)
			node->outfile = get_fd(node->outfile, next[0], 1, 0);
		else
			node->outfile = get_fd(node->outfile, NULL, 1, 0);
	}
	ft_free_matrix(&next);
	return (node);
}

t_mini	*get_outfile2(t_mini *node, char **args, char **arg, int ij[2])
{
	char	**next;

	next = NULL;
	if (arg[++ij[1] + 1])
		node->outfile = get_fd(node->outfile, &arg[++ij[1]][0], 1, 1);
	else
	{
		next = ft_subsplit(args[ij[0] + 1], "<>");
		ij[0] += ft_matrixlen(next) == 1;
		if (next)
			node->outfile = get_fd(node->outfile, next[0], 1, 1);
		else
			node->outfile = get_fd(node->outfile, NULL, 1, 1);
	}
	ft_free_matrix(&next);
	return (node);
}

t_mini	*get_infile1(t_mini *node, char **args, char **arg, int ij[2])
{
	char	**next;

	next = NULL;
	if (arg[ij[1] + 1])
		node->infile = get_fd(node->infile, &arg[++ij[1]][0], 0, 0);
	else
	{
		next = ft_subsplit(args[ij[0] + 1], "<>");
		ij[0] += ft_matrixlen(next) == 1;
		if (next)
			node->infile = get_fd(node->infile, next[0], 0, 0);
		else
			node->infile = get_fd(node->infile, NULL, 0, 0);
	}
	ft_free_matrix(&next);
	return (node);
}

t_mini	*get_infile2(t_mini *node, char **args, char **arg, int ij[2])
{
	char	*limiter;
	char	*warn;
	char	**next;
	char	*temp;

	next = NULL;
	limiter = NULL;
	warn = "minishell: warning: here-document delimited by end-of-file";
	if (arg[++ij[1] + 1])
		limiter = &arg[++ij[1]][0];
	else if (args[ij[0] + 1])
	{
		next = ft_subsplit(args[ij[0] + 1], "<>");
		temp = args[ij[0] + 1];
		args[ij[0] + 1] = ft_substr(args[ij[0] + 1], ft_strlen(next[0]), \
			ft_strlen(args[ij[0] + 1]));
		free(temp);
		limiter = next[0];
		ij[0] += ft_matrixlen(next) == 1;
	}
	node->infile = get_here_doc(NULL, NULL, limiter, warn);
	ft_free_matrix(&next);
	return (node);
}
