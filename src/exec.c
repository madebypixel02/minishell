/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/19 18:43:32 by mbueno-g         ###   ########.fr       */
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

void	get_cmd(t_prompt *prompt, t_list *cmd, char **split_path, char *path)
{
	t_mini	*node;

	node = cmd->content;
	if (node->full_cmd && ft_strchr(*node->full_cmd, '/') && \
		!access(*node->full_cmd, X_OK))
	{
		split_path = ft_split(*node->full_cmd, '/');
		node->full_path = ft_strdup(*node->full_cmd);
		free(node->full_cmd[0]);
		node->full_cmd[0] = \
			ft_strdup(split_path[ft_matrixlen(split_path) - 1]);
	}
	else if (node->full_cmd)
	{
		path = mini_getenv("PATH", prompt->envp, 4);
		split_path = ft_split(path, ':');
		free(path);
		node->full_path = find_command(split_path, *node->full_cmd, \
			node->full_path);
		if (!node->full_path || !node->full_cmd[0] || !node->full_cmd[0][0])
			mini_perror(prompt, NCMD, *node->full_cmd);
	}
	ft_free_matrix(&split_path);
}

static void	*child_redir(t_list *cmd, int fd[2])
{
	t_mini	*node;

	node = cmd->content;
	if (node->infile != STDIN_FILENO)
	{
		dup2(node->infile, STDIN_FILENO);
		close(node->infile);
	}
	if (node->outfile != STDOUT_FILENO)
	{
		dup2(node->outfile, STDOUT_FILENO);
		close(node->outfile);
	}
	else if (cmd->next)
		dup2(fd[WRITE_END], STDOUT_FILENO);
	close(fd[WRITE_END]);
	return ("");
}

void	*child_process(t_prompt *prompt, t_list *cmd, int fd[2], int pidfd[2])
{
	t_mini	*n;
	int		l;
	pid_t	pid;

	pid = getpid();
	n = cmd->content;
	l = 0;
	if (n->full_cmd)
		l = ft_strlen(*n->full_cmd);
	child_redir(cmd, fd);
	close(fd[READ_END]);
	close(pidfd[READ_END]);
	write(pidfd[WRITE_END], &pid, sizeof(int));
	close(pidfd[WRITE_END]);
	child_builtin(prompt, n, l, cmd);
	ft_lstclear(&prompt->cmds, free_content);
	exit(prompt->e_status);
}

void	exec_cmd(t_prompt *prompt, t_list *cmd)
{
	int		fd[2];
	int		pidfd[2];
	t_mini	*node;

	node = cmd->content;
	pipe(fd);
	pipe(pidfd);
	if (((t_mini *)cmd->content)->infile != -1 && \
		((t_mini *)cmd->content)->outfile != -1 && \
		((t_mini *)cmd->content)->full_cmd)
	{
		if (!fork())
			child_process(prompt, cmd, fd, pidfd);
	}
	else
		prompt->e_status = 1;
	close(pidfd[WRITE_END]);
	read(pidfd[READ_END], &node->pid, sizeof(int));
	close(pidfd[READ_END]);
	close(fd[WRITE_END]);
	if (cmd->next && !((t_mini *)cmd->next->content)->infile)
		((t_mini *)cmd->next->content)->infile = fd[READ_END];
	else
		close(fd[READ_END]);
}
