/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/11 23:43:17 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_mini	*get_outfile1(t_mini *node, char **args, int *i)
{
	if (args[*i + 1])
		node->outfile = get_fd(node->outfile, args[++(*i)], 1, 0);
	else
		ft_putstr_fd("Redir Error!\n", 2);
	return (node);
}

t_mini	*get_outfile2(t_mini *node, char **args, int *i)
{
	if (args[++(*i) + 1])
		node->outfile = get_fd(node->outfile, args[++(*i)], 1, 1);
	else
		ft_putstr_fd("Redir Error!\n", 2);
	return (node);
}

t_mini	*get_infile1(t_mini *node, char **args, int *i)
{
	if (args[*i + 1])
		node->infile = get_fd(node->infile, args[++(*i)], 0, 0);
	else
		ft_putstr_fd("Redir Error!\n", 2);
	return (node);
}

t_mini	*get_infile2(t_mini *node, char **args, int *i)
{
	char	*limiter;
	char	*warn;

	limiter = NULL;
	warn = "minishell: warning: here-document delimited by end-of-file";
	if (args[++(*i) + 1])
	{
		limiter = args[++(*i)];
		node->infile = get_here_doc(NULL, NULL, limiter, warn);
	}
	else
		ft_putstr_fd("Redir Error!\n", 2);
	return (node);
}
