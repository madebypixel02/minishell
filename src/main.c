/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/04 18:04:39 by mbueno-g         ###   ########.fr       */
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

char	**check_args(char **args, char *out)
{
	if (!args && out)
		printf("minishell: error while looking for matching quote\n");
	if (!out || (args && builtin(ft_matrixlen(args), args, NULL) == -1))
	{
		ft_free_matrix(&args);
		printf("exit\n");
		return (NULL);
	}
	ft_free_matrix(&args);
	return (args);
}

int	main(void)
{
	char				*str;
	char				*out;
	char				**args;
	struct sigaction	sa;

	out = NULL;
	str = mini_getuser();
	sa.sa_sigaction = handle_sigint;
	while (1)
	{
		sigaction(SIGINT, &sa, NULL);
		out = readline(str);
		if (!out)
		{
			free(str);
			return (0);
		}
		add_history(out);
		args = ft_cmdtrim(out, " ><");
		if (!check_args(args, out) && !out)
			break ;
		free(out);
	}
	free(out);
	free(str);
}
