/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:51:24 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/21 17:34:02 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*find_command(char **env_path, char *cmd, char *full_path)
{
	char	*temp;
	int		i;

	i = -1;
	full_path = NULL;
	while (env_path && env_path[++i])
	{
		free(full_path);
		temp = ft_strjoin(env_path[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
			break ;
	}
	if (!env_path || !env_path[i])
	{
		free(full_path);
		return (NULL);
	}
	return (full_path);
}

void	get_cmd(t_prompt *prompt, t_list *cmd, char **s, char *path)
{
	t_mini	*node;

	node = cmd->content;
	if (node->full_cmd && ft_strchr(*node->full_cmd, '/'))
	{
		s = ft_split(*node->full_cmd, '/');
		node->full_path = ft_strdup(*node->full_cmd);
		free(node->full_cmd[0]);
		node->full_cmd[0] = ft_strdup(s[ft_matrixlen(s) - 1]);
	}
	else if (node->full_cmd)
	{
		path = mini_getenv("PATH", prompt->envp, 4);
		s = ft_split(path, ':');
		free(path);
		node->full_path = find_command(s, *node->full_cmd, \
			node->full_path);
		if (!node->full_path || !node->full_cmd[0] || !node->full_cmd[0][0])
			mini_perror(prompt, NCMD, *node->full_cmd);
	}
	if (node->full_path && access(node->full_path, F_OK) == -1)
		mini_perror(prompt, NDIR, node->full_path);
	else if (node->full_path && access(node->full_path, X_OK) == -1)
		mini_perror(prompt, NPERM, node->full_path);
	ft_free_matrix(&s);
}
