/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/13 19:32:34 by aperez-b         ###   ########.fr       */
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

static t_mini	*get_params(t_mini *node, char **args, char **temp, int *i)
{
	if (args[*i][0] == '>' && args[*i + 1] && args[*i + 1][0] == '>')
		node = get_outfile2(node, temp, i);
	else if (args[*i][0] == '>')
		node = get_outfile1(node, temp, i);
	else if (args[*i][0] == '<' && args[*i + 1] && args[*i + 1][0] == '<')
		node = get_infile2(node, temp, i);
	else if (args[*i][0] == '<')
		node = get_infile1(node, temp, i);
	else if (args[*i][0] != '|')
		node->full_cmd = ft_extend_matrix(node->full_cmd, temp[*i]);
	else
		*i = -2;
	return (node);
}

static char	**get_trimmed(char **args)
{
	char	**temp;
	char	*aux;
	int		j;

	j = -1;
	temp = ft_dup_matrix(args);
	while (temp && temp[++j])
	{
		aux = ft_strtrim_all(temp[j], 0, 0);
		free(temp[j]);
		temp[j] = aux;
	}
	return (temp);
}

static t_list	*stop_fill(t_list *cmds, char **args, char **temp, int i)
{
	if (i == -2)
		mini_perror(PIPERR, NULL);
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (NULL);
}

t_list	*fill_nodes(char **args)
{
	t_list	*cmds[2];
	char	**temp;
	int		i;

	cmds[0] = NULL;
	i = -1;
	temp = get_trimmed(args);
	while (args[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			i += args[i][0] == '|';
			ft_lstadd_back(&cmds[0], ft_lstnew(mini_init()));
			cmds[1] = ft_lstlast(cmds[0]);
		}
		cmds[1]->content = get_params(cmds[1]->content, args, temp, &i);
		if (i < 0)
			return (stop_fill(cmds[0], args, temp, i));
		if (!args[i])
			break ;
	}
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (cmds[0]);
}
