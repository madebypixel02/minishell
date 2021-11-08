/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/08 19:35:41 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdio.h>

int	builtin(int argc, t_list *cmds, char **envp)
{
	int		n;
	char	**argv;

	argv = ((t_mini *)cmds->content)->full_cmd;
	if (!argv)
		return (0);
	n = ft_strlen(*argv);
	if (!ft_strncmp(*argv, "cd", n) && n == 2)
		return (cd(argv));
	else if (!ft_strncmp(*argv, "exit", n) && n == 4)
		return (-1);
	else if (!ft_strncmp(*argv, "pwd", n) && n == 3)
		return (pwd(argc));
	else if (!ft_strncmp(*argv, "echo", n) && n == 4)
		return (echo(argv));
	else if (!ft_strncmp(*argv, "env", n) && n == 3)
		return (env(argc, envp));
	else
		get_cmd(((t_mini *)cmds->content));
	if (((t_mini *)cmds->content)->full_path)
		return (exec_cmd(cmds, envp));
	return (0);
}

int	cd(char **argv)
{
	char	*home_with_path;
	char	*home_dir;

	home_dir = getenv("HOME");
	if (!argv[1])
		chdir(home_dir);
	else if (argv[1][0] == '~')
	{
		home_with_path = ft_strjoin(home_dir, argv[1] + 1);
		if (access(home_with_path, F_OK) != -1)
			chdir(home_with_path);
		else
			printf("minishell: cd: %s: No such file or directory\n", \
				home_with_path);
		free(home_with_path);
	}
	else if (access(argv[1], F_OK) != -1)
		chdir(argv[1]);
	else
		printf("minishell: cd: %s: No such file or directory\n", argv[1]);
	return (0);
}

int	pwd(int argc)
{
	char	*buf;

	if (argc != 1)
		return (printf("pwd: too many arguments\n"));
	buf = getcwd(NULL, 0);
	printf("%s\n", buf);
	free(buf);
	return (0);
}

int	echo(char **argv)
{
	argv++;
	while (*argv)
	{
		if (!*(argv + 1))
			printf("%s", *argv);
		else
			printf("%s ", *argv);
		argv++;
	}
	printf("\n");
	return (0);
}

int	env(int argc, char **envp)
{
	(void)argc;
	ft_putmatrix_fd(envp, 1);
	return (0);
}
