/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/28 17:40:13 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sigint(int sig, siginfo_t *info, void *context)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
	(void)info;
	(void)context;
}

int	main(void)
{
	char				*str;
	char				*out;
	char				**args;
	struct sigaction	sa;

	out = NULL;
	str = "minishell$ ";
	sa.sa_sigaction = handle_sigint;
	while (1)
	{
		sigaction(SIGINT, &sa, NULL);
		out = readline(str);
		if (!out)
			return (0);
		add_history(out);
		args = ft_cmdtrim(out, ' ');
		free(out);
		if (args && builtin(ft_matrixlen(args), args, NULL) == -1)
		{
			ft_free_matrix(&args);
			return (printf("exit\n"));
		}
		ft_free_matrix(&args);
	}
}
