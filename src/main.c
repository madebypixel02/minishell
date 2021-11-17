/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/17 19:24:25 by mbueno-g         ###   ########.fr       */
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

static char	**split_all(char **args, t_prompt *prompt)
{
	char	**subsplit;
	int		i;
	int		quotes[2];

	i = -1;
	while (args && args[++i])
	{
		args[i] = expand_vars(args[i], -1, quotes, prompt);
		args[i] = expand_path(args[i], -1, quotes, \
			mini_getenv("HOME", prompt->envp, 4));
		subsplit = ft_cmdsubsplit(args[i], "<|>");
		ft_matrix_replace_in(&args, subsplit, i);
		i += ft_matrixlen(subsplit) - 1;
		ft_free_matrix(&subsplit);
	}
	return (args);
}

static void	*check_args(char *out, t_prompt *prompt)
{
	char	**args;

	if (out[0] != '\0')
		add_history(out);
	args = ft_cmdtrim(out, " ");
	free(out);
	if (!args)
	{
		mini_perror(prompt, QUOTE, NULL);
		return ("");
	}
	prompt->cmds = fill_nodes(prompt, split_all(args, prompt), -1);
	if (!prompt->cmds)
		return ("");
	prompt->e_status = builtin(prompt, prompt->cmds);
	if (args && prompt->e_status == -1)
	{
		prompt->e_status = prompt->e_status != -1;
		printf("exit\n");
		ft_lstclear(&prompt->cmds, free_content);
		return (NULL);
	}
	ft_lstclear(&prompt->cmds, free_content);
	return ("");
}

static t_prompt	init_prompt(char **envp)
{
	t_prompt	prompt;

	prompt.cmds = NULL;
	prompt.envp = ft_dup_matrix(envp);
	prompt.e_status = 0;
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	char				*out;
	struct sigaction	sa;
	t_prompt			prompt;

	sa.sa_sigaction = handle_sigint;
	prompt = init_prompt(envp);
	while (argv && argc)
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
	exit(prompt.e_status);
}
