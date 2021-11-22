/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/22 18:52:58 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_prompt	init_prompt(char **argv, char **envp)
{
	t_prompt	prompt;
	char		*str;
	char		*num;

	prompt.cmds = NULL;
	prompt.envp = ft_dup_matrix(envp);
	str = getcwd(NULL, 0);
	prompt.envp = mini_setenv("PWD", str, prompt.envp, 3);
	free(str);
	str = mini_getenv("SHLVL", envp, 5);
	if (!str || ft_atoi(str) <= 0)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(str) + 1);
	free(str);
	prompt.envp = mini_setenv("SHLVL", num, prompt.envp, 5);
	free(num);
	prompt.envp = mini_setenv("PATH", \
		"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", prompt.envp, 4);
	str = mini_getenv("_", prompt.envp, 1);
	if (!str)
		prompt.envp = mini_setenv("_", argv[0], prompt.envp, 1);
	free(str);
	prompt.e_status = 0;
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
		str = mini_getprompt(prompt);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		if (str)
			out = mini_readline(&prompt, str);
		else
			out = mini_readline(&prompt, "guest@minishell $ ");
		signal(SIGINT, handle_sigint_child);
		signal(SIGQUIT, handle_sigquit);
		free(str);
		if (!check_args(out, &prompt))
			break ;
	}
	ft_free_matrix(&prompt.envp);
	exit(prompt.e_status);
}
