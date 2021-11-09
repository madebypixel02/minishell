/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/09 12:14:24 by mbueno-g         ###   ########.fr       */
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

static void	*check_args(char *out, char **envp)
{
	t_list	*cmds;
	t_mini	*node;
	char	**args;

	args = ft_cmdtrim(out, " ");
	add_history(out);
	if (!args)
	{
		mini_perror(QUOTE, NULL);
		return (out);
	}
	cmds = parse_args(args);
	node = (t_mini *)cmds->content;
	if (args && builtin(ft_lstsize(cmds), cmds, envp) == -1)
	{
		ft_free_matrix(&args);
		mini_perror(REXIT, NULL);
		return (NULL);
	}
	ft_free_matrix(&args);
	free(node->full_path);
	ft_free_matrix(&node->full_cmd);
	ft_lstclear(&cmds, free);
	return (out);
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	char				*out;
	struct sigaction	sa;

	out = NULL;
	(void)argc;
	(void)argv;
	str = mini_getuser();
	sa.sa_sigaction = handle_sigint;
	while (1)
	{
		sigaction(SIGINT, &sa, NULL);
		out = readline(str);
		if (!out)
		{
			free(str);
			mini_perror(WEXIT, NULL);
			return (0);
		}
		if (!check_args(out, envp))
			break ;
		free(out);
	}
	free(out);
	free(str);
}
