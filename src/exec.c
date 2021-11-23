/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/23 15:43:52 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	*child_redir(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*node;

	node = cmd->content;
	if (node->infile != STDIN_FILENO)
	{
		if (dup2(node->infile, STDIN_FILENO) == -1)
			return (mini_perror(prompt, DUPERR, NULL));
		close(node->infile);
	}
	if (node->outfile != STDOUT_FILENO)
	{
		if (dup2(node->outfile, STDOUT_FILENO) == -1)
			return (mini_perror(prompt, DUPERR, NULL));
		close(node->outfile);
	}
	else if (cmd->next && dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
		return (mini_perror(prompt, DUPERR, NULL));
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
	child_redir(prompt, cmd, fd);
	close(fd[READ_END]);
	close(pidfd[READ_END]);
	write(pidfd[WRITE_END], &pid, sizeof(int));
	close(pidfd[WRITE_END]);
	child_builtin(prompt, n, l, cmd);
	ft_lstclear(&prompt->cmds, free_content);
	exit(prompt->e_status);
}

void	*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2], int pidfd[2])
{
	t_mini	*node;
	pid_t	pid;

	node = cmd->content;
	if (node->infile == -1 || node->outfile == -1)
		return (NULL);
	if (node->full_path && access(node->full_path, X_OK) == 0)
	{
		pid = fork();
		if (pid < 0)
		{
			close(fd[READ_END]);
			close(fd[WRITE_END]);
			close(pidfd[READ_END]);
			close(pidfd[WRITE_END]);
			return (mini_perror(prompt, FORKERR, NULL));
		}
		else if (!pid)
			child_process(prompt, cmd, fd, pidfd);
	}
	else if (node->full_path && access(node->full_path, F_OK) == 0)
		prompt->e_status = 126;
	else if (node->full_cmd)
		prompt->e_status = 127;
	return ("");
}

void	*exec_cmd(t_prompt *prompt, t_list *cmd)
{
	int		fd[2];
	int		pidfd[2];
	t_mini	*node;

	get_cmd(prompt, cmd, NULL, NULL);
	node = cmd->content;
	if (pipe(fd) == -1)
		return (mini_perror(prompt, PIPERR, NULL));
	if (pipe(pidfd) == -1)
	{
		close(fd[READ_END]);
		close(fd[WRITE_END]);
		return (mini_perror(prompt, PIPERR, NULL));
	}
	if (!check_to_fork(prompt, cmd, fd, pidfd))
		return (NULL);
	close(pidfd[WRITE_END]);
	read(pidfd[READ_END], &node->pid, sizeof(int));
	close(pidfd[READ_END]);
	close(fd[WRITE_END]);
	if (cmd->next && !((t_mini *)cmd->next->content)->infile)
		((t_mini *)cmd->next->content)->infile = fd[READ_END];
	else
		close(fd[READ_END]);
	return (NULL);
}
