/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/19 13:45:44 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
//	t_list	*aux;

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
//	aux = prompt->cmds;
	//while (aux && aux->next)
	//{
		waitpid((((t_mini *)ft_lstlast(prompt->cmds)->content))->pid, &prompt->e_status, 0);
//		aux = aux->next;
//	}
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
	t_prompt			prompt;

	prompt = init_prompt(envp);
	ioctl(1, 0x03);
	while (argv && argc)
	{
		str = mini_getprompt(prompt);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		out = readline(str);
		signal(SIGINT, handle_sigint_child);
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
