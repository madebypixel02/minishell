/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/18 16:01:15 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_fd(t_prompt *prompt, int oldfd, char *path, int flags[2])
{
	int	fd;

	if (!path)
		return (-1);
	if (oldfd > 2)
		close(oldfd);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else if (flags[0] && !flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (!flags[0] && oldfd != -1)
		fd = open(path, O_RDONLY);
	else
		fd = oldfd;
	if (fd == -1 && oldfd != -1)
		mini_perror(prompt, NDIR, path);
	return (fd);
}

t_mini	*get_outfile1(t_prompt *prompt, t_mini *node, char **args, int *i)
{
	char	*nl;
	int		flags[2];

	flags[0] = 1;
	flags[1] = 0;
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->outfile = get_fd(prompt, node->outfile, args[*i], flags);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
		prompt->e_status = 1;
	}
	return (node);
}

t_mini	*get_outfile2(t_prompt *prompt, t_mini *node, char **args, int *i)
{
	char	*nl;
	int		flags[2];

	flags[0] = 1;
	flags[1] = 1;
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
		node->outfile = get_fd(prompt, node->outfile, args[*i], flags);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
		prompt->e_status = 1;
	}
	return (node);
}

t_mini	*get_infile1(t_prompt *prompt, t_mini *node, char **args, int *i)
{
	char	*nl;
	int		flags[2];

	flags[0] = 0;
	flags[1] = 0;
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->infile = get_fd(prompt, node->infile, args[*i], flags);
	if (!args[*i] || node->infile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
		prompt->e_status = 1;
	}
	return (node);
}

t_mini	*get_infile2(t_prompt *prompt, t_mini *node, char **args, int *i)
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
	if (!args[*i] || node->infile == -1)
	{
		*i = -1;
		ft_putendl_fd(nl, 2);
		prompt->e_status = 1;
	}
	return (node);
}
