/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/30 17:30:04 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd)
{
	if (n->full_cmd && !ft_strncmp(*n->full_cmd, "pwd", l) && l == 3)
		prompt->e_status = mini_pwd();
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "echo", l) && l == 4)
		prompt->e_status = mini_echo(cmd);
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "env", l) && l == 3)
	{
		ft_putmatrix_fd(prompt->envp, 1);
		prompt->e_status = 0;
	}
	else if (n->full_cmd)
		execve(n->full_path, n->full_cmd, prompt->envp);
}

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

void	*child_process(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*n;
	int		l;

	n = cmd->content;
	l = 0;
	if (n->full_cmd)
		l = ft_strlen(*n->full_cmd);
	child_redir(prompt, cmd, fd);
	close(fd[READ_END]);
	child_builtin(prompt, n, l, cmd);
	ft_lstclear(&prompt->cmds, free_content);
	exit(prompt->e_status);
}

void	*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*n;
	pid_t	pid;

	n = cmd->content;
	if (n->infile == -1 || n->outfile == -1)
		return (NULL);
	if (is_builtin(n) || (n->full_path && access(n->full_path, X_OK) == 0))
	{
		pid = fork();
		if (pid < 0)
		{
			close(fd[READ_END]);
			close(fd[WRITE_END]);
			return (mini_perror(prompt, FORKERR, NULL));
		}
		else if (!pid)
			child_process(prompt, cmd, fd);
		if (!cmd->next)
			waitpid(pid, &prompt->e_status, 0);
	}
	else if (!is_builtin(n) && n->full_path && access(n->full_path, F_OK) == 0)
		prompt->e_status = 126;
	else if (!is_builtin(n) && n->full_cmd)
		prompt->e_status = 127;
	return ("");
}

void	*exec_cmd(t_prompt *prompt, t_list *cmd)
{
	int		fd[2];

	get_cmd(prompt, cmd, NULL, NULL);
	if (pipe(fd) == -1)
		return (mini_perror(prompt, PIPERR, NULL));
	if (!check_to_fork(prompt, cmd, fd))
		return (NULL);
	close(fd[WRITE_END]);
	if (cmd->next && !((t_mini *)cmd->next->content)->infile)
		((t_mini *)cmd->next->content)->infile = fd[READ_END];
	else
		close(fd[READ_END]);
	return (NULL);
}
