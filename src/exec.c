/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/14 21:27:42 by aperez-b         ###   ########.fr       */
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

static void	*child_process(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*n;
	int		l;

	n = cmd->content;
	l = 0;
	if (n->full_cmd)
		l = ft_strlen(*n->full_cmd);
	child_redir(cmd, fd);
	close(fd[READ_END]);
	if (n->full_cmd && !ft_strncmp(*n->full_cmd, "pwd", l) && l == 3)
		prompt->e_status = mini_pwd();
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "echo", l) && l == 4)
		prompt->e_status = mini_echo(cmd);
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "env", l) && l == 3)
		prompt->e_status = mini_env(prompt);
	else
	{
		get_cmd(prompt, cmd, NULL, NULL);
		if (n->full_cmd && n->full_path)
			execve(n->full_path, n->full_cmd, prompt->envp);
		prompt->e_status = 1;
	}
	ft_lstclear(&prompt->cmds, free_content);
	exit(prompt->e_status);
}

void	exec_cmd(t_prompt *prompt, t_list *cmd)
{
	pid_t	pid;
	int		fd[2];

	pipe(fd);
	if (((t_mini *)cmd->content)->infile != -1 && \
		((t_mini *)cmd->content)->outfile != -1 && \
		((t_mini *)cmd->content)->full_cmd)
	{
		pid = fork();
		if (!pid)
			child_process(prompt, cmd, fd);
		waitpid(pid, &prompt->e_status, 0);
	}
	else
		prompt->e_status = 1;
	close(fd[WRITE_END]);
	if (cmd->next && !((t_mini *)cmd->next->content)->infile)
		((t_mini *)cmd->next->content)->infile = fd[READ_END];
	else
		close(fd[READ_END]);
}
