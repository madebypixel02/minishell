/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/12/30 14:45:47 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!is_builtin(n) && n->full_cmd)
		execve(n->full_path, n->full_cmd, prompt->envp);
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "pwd", l) \
		&& l == 3)
		prompt->e_status = mini_pwd();
	else if (is_builtin(n) && n->full_cmd && \
		!ft_strncmp(*n->full_cmd, "echo", l) && l == 4)
		prompt->e_status = mini_echo(cmd);
	else if (is_builtin(n) && n->full_cmd && \
		!ft_strncmp(*n->full_cmd, "env", l) && l == 3)
	{
		ft_putmatrix_fd(prompt->envp, 1);
		prompt->e_status = 0;
	}
}

static void	*child_redir(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*node;

	node = cmd->content;
	if (node->infile != STDIN_FILENO)
	{
		if (dup2(node->infile, STDIN_FILENO) == -1)
			return (mini_perror(prompt, DUPERR, NULL, 1));
		close(node->infile);
	}
	if (node->outfile != STDOUT_FILENO)
	{
		if (dup2(node->outfile, STDOUT_FILENO) == -1)
			return (mini_perror(prompt, DUPERR, NULL, 1));
		close(node->outfile);
	}
	else if (cmd->next && dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
		return (mini_perror(prompt, DUPERR, NULL, 1));
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

void	exec_fork(t_prompt *prompt, t_list *cmd, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(fd[READ_END]);
		close(fd[WRITE_END]);
		mini_perror(prompt, FORKERR, NULL, 1);
	}
	else if (!pid)
		child_process(prompt, cmd, fd);
	if (pid >= 0 && !cmd->next)
		waitpid(pid, &prompt->e_status, 0);
}

void	*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2])
{
	t_mini	*n;
	DIR		*dir;

	n = cmd->content;
	dir = NULL;
	if (n->full_cmd)
		dir = opendir(*n->full_cmd);
	if (n->infile == -1 || n->outfile == -1)
		return (NULL);
	if ((n->full_path && access(n->full_path, X_OK) == 0) || is_builtin(n))
		exec_fork(prompt, cmd, fd);
	else if (!is_builtin(n) && ((n->full_path && \
		!access(n->full_path, F_OK)) || dir))
		prompt->e_status = 126;
	else if (!is_builtin(n) && n->full_cmd)
		prompt->e_status = 127;
	if (dir)
		closedir(dir);
	return ("");
}
