/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/26 19:57:40 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sigint(int sig, siginfo_t *info, void *context)
{
	write(1, "\n",1);
	rl_replace_line("",0);
	rl_on_new_line();
	rl_redisplay();
	(void) context;
	//printf("Cakjjk %d", sig);
	//exit(0);
}

int	main(void)
{
	char	*str;
	char	*out;
	char	**args;
	struct sigaction sa;

	out = NULL;
	str = "minishell$ ";
	sa.sa_sigaction = handle_sigint;
	while(1)
	{
		sigaction(SIGINT, &sa, NULL);
		out = readline(str);
		add_history(out);
		args = ft_split(out, ' ');
		free(out);
		if (!args || builtin(ft_matrixlen(args), args, NULL))
		{
			ft_free_matrix(&args);
			return (printf("exit\n"));
		}
		ft_free_matrix(&args);
	}
}
