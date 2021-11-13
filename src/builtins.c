/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/13 18:45:41 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin(t_prompt *prompt)
{
	int		n;
	char	**argv;

	argv = ((t_mini *)prompt->cmds->content)->full_cmd;
	n = ft_strlen(*argv);
	if (!ft_strncmp(*argv, "exit", n) && n == 4)
		return (-1);
	else if (!ft_strncmp(*argv, "cd", n) && n == 2)
		return (mini_cd(prompt));
	else if (!ft_strncmp(*argv, "pwd", n) && n == 3)
		return (mini_pwd(prompt));
	else if (!ft_strncmp(*argv, "echo", n) && n == 4)
		return (mini_echo(prompt));
	else if (!ft_strncmp(*argv, "env", n) && n == 3)
		return (mini_env(prompt));
	else if (!ft_strncmp(*argv, "export", n) && n == 6)
		return (mini_export(prompt));
	else if (!ft_strncmp(*argv, "unset", n) && n == 5)
		return (mini_unset(prompt));
	else
		get_cmd(prompt, prompt->cmds, NULL, NULL);
	return (exec_cmd(prompt));
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
		mini_perror(NDIR, argv[1]);
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

int	mini_pwd(t_prompt *prompt)
{
	char	*buf;
	t_mini	*node;

	node = prompt->cmds->content;
	buf = getcwd(NULL, 0);
	ft_putendl_fd(buf, node->outfile);
	free(buf);
	return (0);
}

int	mini_echo(t_prompt *prompt)
{
	int		newline;
	int		i;
	char	**argv;
	t_mini	*node;

	i = 0;
	argv = ((t_mini *)prompt->cmds->content)->full_cmd;
	newline = 1;
	node = prompt->cmds->content;
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

int	mini_env(t_prompt *prompt)
{
	int		i;
	t_mini	*node;

	i = 0;
	node = prompt->cmds->content;
	while (prompt->envp[i])
	{
		ft_putendl_fd(prompt->envp[i], node->outfile);
		i++;
	}
	return (0);
}
