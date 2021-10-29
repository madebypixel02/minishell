/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/29 11:19:02 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin(int argc, char **argv, char **envp)
{
	int	n;

	n = ft_strlen(*argv);
	if (!ft_strncmp(*argv, "cd", n) && n == 2)
		return(cd(argv));
	if (!ft_strncmp(*argv, "exit", n) && n == 4)
		return (-1);
	if (!ft_strncmp(*argv, "pwd", n) && n == 3)
		return (pwd(argc));
	if (!ft_strncmp(*argv, "echo", n) && n == 4)
		return (echo(argv));
	if (!ft_strncmp(*argv, "env", n) && n == 3)
		return (env(argc, envp));
	argc = 0;
	return (argc);
}

int	cd(char **argv)
{
	char	*user;
	char	*home_dir;
	char	*home_with_path;

	user = getenv("USER");
	home_dir = ft_strjoin("/Users/", user);
	if (access(home_dir, F_OK) == -1)
	{
		free(home_dir);
		home_dir = ft_strjoin("/home/", user);
	}
	if (argv[1] && access(argv[1], F_OK) != -1)
		chdir(argv[1]);
	if (!argv[1])
		chdir(home_dir);
	else if (argv[1][0] == '~')
	{
		home_with_path = ft_strjoin(home_dir, argv[1] + 1);
		chdir(home_with_path);
		free(home_with_path);
	}
	free(home_dir);
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
	while(*argv)
	{
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
