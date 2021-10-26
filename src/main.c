/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/26 17:50:43 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sigint(int sig)//, siginfo_t *info, void *context)
{
	write(1, "\n",1);
	rl_replace_line("",0);
	rl_display();
	//printf("Cakjjk %d", sig);
	//exit(0);
}

void	test(void)
{
	char	*str;
	char	*out;
	char	**args;
	struct sigaction sa;

	printf("entrasss\n");

	out = NULL;
	str = "minishell$ ";
	//sa.sa_sigaction = handle_sigint;
	//sigaction(SIGINT, &sa, NULL);
	signal(SIGINT, handle_sigint);
	//sigaction(SIGQUIT, &sa, NULL);
	out = readline(str);
	add_history(out);
	args = ft_split(out, ' ');
	free(out);
	if (builtin(ft_matrixlen(args), args, NULL))
	{
		printf("hoaaaa\n");
		ft_free_matrix(&args);
		//return (printf("exit\n"));
		exit(0);
	}
	ft_free_matrix(&args);
	printf("dkg\n");
	/*if (pid > 0)
	{
		waitpid(pid, NULL,0);
		test();	
	}*/
	test();	
}


int	main(int argc, char **argv, char **envp)
{
	test();
}	
