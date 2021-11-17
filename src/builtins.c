/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/17 20:10:12 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin(t_prompt *prompt, t_list *cmd)
{
	int		n;
	char	**a;

	while (cmd)
	{
		a = ((t_mini *)cmd->content)->full_cmd;
		n = 0;
		if (a)
			n = ft_strlen(*a);
		if (a && !ft_strncmp(*a, "exit", n) && n == 4)
			return (-1);
		else if (!cmd->next && a && !ft_strncmp(*a, "cd", n) && n == 2)
			prompt->e_status = mini_cd(prompt);
		else if (!cmd->next && a && !ft_strncmp(*a, "export", n) && n == 6)
			prompt->e_status = mini_export(prompt);
		else if (!cmd->next && a && !ft_strncmp(*a, "unset", n) && n == 5)
			prompt->e_status = mini_unset(prompt);
		else
			exec_cmd(prompt, cmd);
		cmd = cmd->next;
	}
	return (prompt->e_status);
}

int	mini_cd(t_prompt *prompt)
{
	char	*home_dir;
	char	**argv;
	char	*pwd;

	argv = ((t_mini *)prompt->cmds->content)->full_cmd;
	pwd = getcwd(NULL, 0);
	home_dir = mini_getenv("HOME", prompt->envp, 4);
	if (!argv[1])
		chdir(home_dir);
	free(home_dir);
	if (argv[1] && access(argv[1], F_OK) != -1)
		chdir(argv[1]);
	else if (argv[1])
	{
		mini_perror(prompt, NDIR, argv[1]);
		free(pwd);
		return (1);
	}
	prompt->envp = mini_setenv("OLDPWD", pwd, prompt->envp, 6);
	free(pwd);
	pwd = getcwd(NULL, 0);
	prompt->envp = mini_setenv("PWD", pwd, prompt->envp, 3);
	free(pwd);
	return (0);
}

int	mini_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}

int	mini_echo(t_list *cmd)
{
	int		newline;
	int		i;
	char	**argv;
	t_mini	*node;

	i = 0;
	newline = 1;
	node = cmd->content;
	argv = node->full_cmd;
	while (argv && argv[++i])
	{
		if (i == 1 && !ft_strncmp(argv[i], "-n", 3))
			newline = 0;
		else
		{
			ft_putstr_fd(argv[i], 1);
			if (argv[i + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

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
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "cd", l) && l == 2)
		prompt->e_status = mini_cd(prompt);
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "export", l) && l == 6)
		prompt->e_status = mini_export(prompt);
	else if (n->full_cmd && !ft_strncmp(*n->full_cmd, "unset", l) && l == 5)
		prompt->e_status = mini_unset(prompt);
	else
	{
		get_cmd(prompt, cmd, NULL, NULL);
		if (n->full_cmd && n->full_path)
			execve(n->full_path, n->full_cmd, prompt->envp);
		prompt->e_status = 1;
	}
}
