/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/13 19:26:32 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_fd(int oldfd, char *path, int is_outfile, int append)
{
	int	fd;

	if (!path)
		return (-1);
	if (oldfd > 2)
		close(oldfd);
	if (is_outfile && append)
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (is_outfile && !append)
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (!is_outfile)
		fd = open(path, O_RDONLY);
	return (fd);
}

t_mini	*get_outfile1(t_mini *node, char **args, int *i)
{
	char	*nl;

	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->outfile = get_fd(node->outfile, args[*i], 1, 0);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
	}
	return (node);
}

t_mini	*get_outfile2(t_mini *node, char **args, int *i)
{
	char	*nl;

	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
		node->outfile = get_fd(node->outfile, args[*i], 1, 1);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
	}
	return (node);
}

t_mini	*get_infile1(t_mini *node, char **args, int *i)
{
	char	*nl;

	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->infile = get_fd(node->infile, args[*i], 0, 0);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
	}
	return (node);
}

t_mini	*get_infile2(t_mini *node, char **args, int *i)
{
	char	*limiter;
	char	*warn;
	char	*nl;

	limiter = NULL;
	warn = "minishell: warning: here-document delimited by end-of-file";
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
	{
		limiter = args[*i];
		node->infile = get_here_doc(NULL, NULL, limiter, warn);
	}
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
	}
	return (node);
}
