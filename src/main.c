/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2022/03/07 23:43:41 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int	g_status;

static void	mini_getpid(t_prompt *p)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		mini_perror(FORKERR, NULL, 1);
		ft_free_matrix(&p->envp);
		exit(1);
	}
	if (!pid)
	{
		ft_free_matrix(&p->envp);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	p->pid = pid - 1;
}

static t_prompt	init_vars(t_prompt prompt, char *str, char **argv)
{
	char	*num;

	str = getcwd(NULL, 0);
	prompt.envp = mini_setenv("PWD", str, prompt.envp, 3);
	free(str);
	str = mini_getenv("SHLVL", prompt.envp, 5);
	if (!str || ft_atoi(str) <= 0)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(str) + 1);
	free(str);
	prompt.envp = mini_setenv("SHLVL", num, prompt.envp, 5);
	free(num);
	str = mini_getenv("PATH", prompt.envp, 4);
	if (!str)
		prompt.envp = mini_setenv("PATH", \
		"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", prompt.envp, 4);
	free(str);
	str = mini_getenv("_", prompt.envp, 1);
	if (!str)
		prompt.envp = mini_setenv("_", argv[0], prompt.envp, 1);
	free(str);
	return (prompt);
}

static t_prompt	init_prompt(char **argv, char **envp)
{
	t_prompt	prompt;
	char		*str;

	str = NULL;
	prompt.cmds = NULL;
	prompt.envp = ft_dup_matrix(envp);
	g_status = 0;
	mini_getpid(&prompt);
	prompt = init_vars(prompt, str, argv);
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	char				*out;
	t_prompt			prompt;

	prompt = init_prompt(argv, envp);
	while (argv && argc)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		str = mini_getprompt(prompt);
		if (str)
			out = readline(str);
		else
			out = readline("guest@minishell $ ");
		free(str);
		if (!check_args(out, &prompt))
			break ;
	}
	exit(g_status);
}
