/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/29 11:19:00 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>

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

char	*mini_getuser(void)
{
	char	*user;
	char	*full;

	user = ft_strjoin(getenv("USER"), "@"); 
	if (!user)
		return (NULL);
	full = ft_strjoin(user, "minishell$ ");
	free(user);
	return (full);
}

int	main(void)
{
	char				*str;
	char				*out;
	char				**args;
	struct sigaction	sa;
	char	*arg[2];

	out = NULL;
	str = mini_getuser();
	sa.sa_sigaction = handle_sigint;
	while (1)
	{
		sigaction(SIGINT, &sa, NULL);
		out = readline(str);
		if (!out)
			break ;
		add_history(out);
		args = ft_cmdtrim(out, ' ');
		//parse_args(args);
		free(out);
		if (args && builtin(ft_matrixlen(args), args, NULL) == -1)
		{
			ft_free_matrix(&args);
			printf("exit\n");
			break ;
		}
		ft_free_matrix(&args);
	}
	free(str);
}
