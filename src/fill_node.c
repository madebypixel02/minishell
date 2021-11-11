/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/12 00:18:39 by aperez-b         ###   ########.fr       */
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

static t_mini	*parse_no_quotes(t_mini *node, char **args, int *i)
{
	if (args[*i][0] == '>' && args[*i + 1] && args[*i + 1][0] == '>')
		node = get_outfile2(node, args, i);
	else if (args[*i][0] == '>')
		node = get_outfile1(node, args, i);
	else if (args[*i][0] == '<' && args[*i + 1] && args[*i + 1][0] == '<')
		node = get_infile2(node, args, i);
	else if (args[*i][0] == '<')
		node = get_infile1(node, args, i);
	else
		node->full_cmd = ft_extend_matrix(node->full_cmd, args[*i]);
	return (node);
}

static t_mini	*get_params(t_mini *node, char **args, int *i, int quotes[2])
{
	char	*new;
	char	*aux;
	char	type;

	new = NULL;
	if (!quotes[0] && !quotes[1])
		node = parse_no_quotes(node, args, i);
	else
	{
		type = args[*i][0];
		while (args[++(*i)] && args[*i][0] != type)
		{
			aux = ft_strjoin(new, args[*i]);
			free(new);
			new = aux;
		}
		if (!args[*i])
		{
			ft_putstr_fd("Quote error!\n", 2);
			return (node);
		}
		node->full_cmd = ft_extend_matrix(node->full_cmd, new);
		free(new);
	}
	return (node);
}

t_list	*fill_nodes(char **args)
{
	int		i;
	int		quotes[2];
	t_list	*aux;
	t_list	*cmds;

	cmds = NULL;
	i = -1;
	quotes[0] = 0;
	quotes[1] = 0;
	while (args[++i])
	{
		aux = ft_lstlast(cmds);
		quotes[0] = (quotes[0] + (!quotes[1] && args[i][0] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && args[i][0] == '\"')) % 2;
		if (i == 0 || (!quotes[0] && !quotes[1] && args[i][0] == '|' && \
			args[i + 1] && args[i + 1][0]))
		{
			ft_lstadd_back(&cmds, ft_lstnew(mini_init()));
			aux = ft_lstlast(cmds);
		}
		aux->content = get_params(aux->content, args, &i, quotes);
		if (!args[i])
			break ;
	}
	return (cmds);
}
