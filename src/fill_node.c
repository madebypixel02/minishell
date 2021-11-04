/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/04 12:38:11 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <unistd.h>

static t_mini	*mini_init(t_mini *mini)
{
	mini->cmd = NULL;
	mini->full_cmd = NULL;
	mini->full_path = NULL;
	mini->infile = STDIN_FILENO;
	mini->outfile = STDOUT_FILENO;
	return (mini);
}

static int	get_fd(int oldfd, char *path, int is_outfile, int append)
{
	int	fd;

	if (!path)
		return (-1);
	if (oldfd > 2)
		close(oldfd);
	printf("Path: %s\n", path);
	if (is_outfile && append)
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (is_outfile && !append)
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (!is_outfile)
		fd = open(path, O_RDONLY);
	return (fd);
}

static	t_mini	*check_arg(t_mini *mini, char **args, int *i)
{
	char	**arg;
	int		j;

	j = 0;
	arg = ft_subsplit(args[*i], "<>");
	ft_putmatrix_fd(arg, 1);
	if (!arg)
		return (NULL);
	while (arg[j])
	{
		if (arg[j][0] == '>' && arg[j + 1] && arg[j + 1][0] == '>')
		{
			if (arg[j + 2])
				mini->outfile = get_fd(mini->outfile, &arg[j + 2][0], 1, 1);
			else
				mini->outfile = get_fd(mini->outfile, args[++(*i)], 1, 1);
		}
		j++;
	}
	ft_free_matrix(&arg);
	return (mini);
}

t_mini	*fill_node(char **args)
{
	t_mini	*node;
	int		i;

	i = 0;
	node = malloc(sizeof(t_mini));
	if (!node)
		return (NULL);
	node = mini_init(node);
	while (args[i])
	{
		node = check_arg(node, args, &i);
		if (args[i])
			i++;
	}
	return (node);
}
