/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/14 12:40:02 by aperez-b         ###   ########.fr       */
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
		else if (a && !ft_strncmp(*a, "cd", n) && n == 2)
			prompt->e_status = mini_cd(prompt);
		else if (a && !ft_strncmp(*a, "export", n) && n == 6)
			prompt->e_status = mini_export(prompt);
		else if (a && !ft_strncmp(*a, "unset", n) && n == 5)
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
	mini_setenv("OLDPWD", pwd, prompt->envp, 6);
	free(pwd);
	pwd = getcwd(NULL, 0);
	mini_setenv("PWD", pwd, prompt->envp, 3);
	free(pwd);
	return (0);
}

int	mini_pwd(t_list *cmd)
{
	char	*buf;
	t_mini	*node;

	node = cmd->content;
	buf = getcwd(NULL, 0);
	ft_putendl_fd(buf, node->outfile);
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
			ft_putstr_fd(argv[i], node->outfile);
			if (argv[i + 1])
				ft_putchar_fd(' ', node->outfile);
		}
	}
	if (newline)
		ft_putchar_fd('\n', node->outfile);
	return (0);
}

int	mini_env(t_prompt *prompt, t_list *cmd)
{
	int		i;
	t_mini	*node;

	i = 0;
	node = cmd->content;
	while (prompt->envp[i])
	{
		ft_putendl_fd(prompt->envp[i], node->outfile);
		i++;
	}
	return (0);
}
