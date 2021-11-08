/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/08 19:35:49 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	pipex_here_fd(char *hdoc_str)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	pid = fork();
	if (!pid)
	{
		close(fd[READ_END]);
		write(fd[WRITE_END], hdoc_str, ft_strlen(hdoc_str));
		close(fd[WRITE_END]);
		free(hdoc_str);
		exit(0);
	}	
	close(fd[WRITE_END]);
	free(hdoc_str);
	return (fd[READ_END]);
}

static int	get_here_doc(char *str, char *full, char *limit, char *warn)
{
	char	*temp;
	size_t	len;

	len = 0;
	while (!str || ft_strncmp(str, limit, len) || ft_strlen(limit) != len)
	{
		temp = full;
		full = ft_strjoin(full, str);
		free(temp);
		free(str);
		str = readline("> ");
		if (!str)
		{
			printf("%s (wanted `%s\')\n", warn, limit);
			break ;
		}
		temp = str;
		str = ft_strjoin(str, "\n");
		free(temp);
		len = ft_strlen(str) - 1;
	}
	free(str);
	return (pipex_here_fd(full));
}

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
