/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/05 07:42:56 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_mini	*get_outfile1(t_mini *node, char **args, char **arg, int ij[2])
{
	if (arg[ij[1] + 1])
		node->outfile = get_fd(node->outfile, &arg[ij[1] + 1][0], 1, 0);
	else
		node->outfile = get_fd(node->outfile, args[++ij[0]], 1, 0);
	return (node);
}

t_mini	*get_outfile2(t_mini *node, char **args, char **arg, int ij[2])
{
	if (arg[ij[1] + 2])
		node->outfile = get_fd(node->outfile, &arg[ij[1] + 2][0], 1, 1);
	else
		node->outfile = get_fd(node->outfile, args[++ij[0]], 1, 1);
	ij[1]++;
	return (node);
}

t_mini	*get_infile1(t_mini *node, char **args, char **arg, int ij[2])
{
	if (arg[ij[1] + 1])
		node->infile = get_fd(node->infile, &arg[ij[1] + 1][0], 0, 0);
	else
		node->infile = get_fd(node->infile, args[++ij[0]], 0, 0);
	return (node);
}

t_mini	*get_params(t_mini *node, char **args, char **arg, int ij[2])
{
	if (arg[ij[1]][0] == '>' && arg[ij[1] + 1] && arg[ij[1] + 1][0] == '>')
		node = get_outfile2(node, args, arg, ij);
	else if (arg[ij[1]][0] == '>')
		node = get_outfile1(node, args, arg, ij);
	else if (arg[ij[1]][0] == '<')
		node = get_infile1(node, args, arg, ij);
	else if (!node->cmd)
	{
		node->cmd = ft_strdup(arg[ij[1]]);
		node->full_cmd = ft_extend_matrix(node->full_cmd, arg[ij[1]]);
	}
	else
		node->full_cmd = ft_extend_matrix(node->full_cmd, arg[ij[1]]);
	return (node);
}
