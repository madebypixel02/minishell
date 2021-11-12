/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/12 19:55:43 by mbueno-g         ###   ########.fr       */
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

static void	*check_args(char *out, t_prompt *prompt)
{
	void	*aux;
	char	**args;

	args = ft_subsplit(out, "\"\'<|> ");
	add_history(out);
	aux = prompt;
	free(out);
	if (!args)
	{
		mini_perror(QUOTE, NULL);
		return (aux);
	}
	prompt->cmds = parse_args(args, prompt);
	if (!prompt->cmds)
		return (aux);
	if (args && builtin(prompt) == -1)
	{
		aux = NULL;
		printf("exit\n");
	}
	ft_lstclear(&prompt->cmds, free_content);
	return (aux);
}

static t_prompt	init_prompt(char **envp)
{
	t_prompt	prompt;

	prompt.cmds = NULL;
	prompt.envp = ft_dup_matrix(envp);
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	char				*out;
	struct sigaction	sa;
	t_prompt			prompt;

	out = NULL;
	(void)argc;
	(void)argv;
	sa.sa_sigaction = handle_sigint;
	prompt = init_prompt(envp);
	while (1)
	{
		str = mini_getprompt(prompt);
		sigaction(SIGINT, &sa, NULL);
		out = readline(str);
		free(str);
		if (!out)
		{
			printf("exit\n");
			break ;
		}
		if (!check_args(out, &prompt))
			break ;
	}
	ft_free_matrix(&prompt.envp);
}
