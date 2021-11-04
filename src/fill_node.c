/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/04 19:50:04 by aperez-b         ###   ########.fr       */
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
			get_params(node, args, arg, ij);
		ft_free_matrix(&arg);
		ij[0] += (args[ij[0]] != NULL);
	}
	return (node);
}
