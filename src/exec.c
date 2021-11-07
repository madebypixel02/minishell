/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/07 16:20:33 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	find_command(char **env_path, char *cmd, char **full_path)
{
	char	*temp;
	int		i;

	i = -1;
	*full_path = NULL;
	while (env_path && env_path[++i])
	{
		free(*full_path);
		temp = ft_strjoin(env_path[i], "/");
		if (!temp)
			return (-2);
		*full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (-2);
		if (access(*full_path, F_OK) == 0)
			break ;
	}
	if (!env_path || !env_path[i])
	{
		free(*full_path);
		*full_path = NULL;
		return (-1);
	}
	return (0);
}

void	get_cmd(t_mini *node)
{
	char	**split_path;
	char	*path;

	if (access(*node->full_cmd, F_OK) == 0)
	{
		split_path = ft_split(*node->full_cmd, '/');
		node->full_path = ft_strdup(*node->full_cmd);
		free(node->full_cmd[0]);
		node->full_cmd[0] = ft_strdup(split_path[ft_matrixlen(split_path) - 1]);
	}
	else
	{
		path = getenv("PATH");
		split_path = ft_split(path, ':');
		find_command(split_path, *node->full_cmd, &node->full_path);
		if (!node->full_path)
			printf("minishell: command not found: %s\n", *node->full_cmd);
	}
	ft_free_matrix(&split_path);
}

int	exec_cmd(t_list *cmd, char **envp)
{
	pid_t	pid;
	t_mini	*node;

	node = cmd->content;
	pid = fork();
	if (!pid)
	{
		dup2(node->infile, STDIN_FILENO);
		if (node->outfile != STDOUT_FILENO)
		{
			dup2(node->outfile, STDOUT_FILENO);
			close(node->outfile);
		}
		execve(node->full_path, node->full_cmd, envp);
		exit(1);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}
