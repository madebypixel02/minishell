/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/26 17:50:44 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin(int argc, char **argv, char **envp)
{
	int	n;

	n = ft_strlen(*argv);
	if (!ft_strncmp(*argv, "cd", n) && n == 2)
		return (cd(argv));
	if (!ft_strncmp(*argv, "exit", n) && n == 4)
		return (1);
	if (!ft_strncmp(*argv, "pwd", n) && n == 3)
		return (pwd(argc));
	if (!ft_strncmp(*argv, "env", n) && n == 3)
		return (env(argc, envp));
	argc = 0;
	return (argc);
}

int	cd(char **argv)
{
	chdir(argv[1]);
	return (0);
}

int	pwd(int argc)
{
	char	*buf;

	if (argc != 1)
		return (printf("pwd: too many arguments\n"));
	buf = getcwd(NULL, 0);
	printf("%s\n", buf);
	return (0);
}

int	env(int argc, char **envp)
{
	(void)argc;
	ft_putmatrix_fd(envp, 1);
	return (0);
}
