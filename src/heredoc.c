/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 10:17:00 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/19 18:31:26 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	minishell_here_fd(char *hdoc_str)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	pid = fork();
	if (!pid)
	{
		close(fd[READ_END]);
		if (hdoc_str)
			write(fd[WRITE_END], hdoc_str, ft_strlen(hdoc_str));
		close(fd[WRITE_END]);
		free(hdoc_str);
		exit(0);
	}
	close(fd[WRITE_END]);
	free(hdoc_str);
	return (fd[READ_END]);
}

int	get_here_doc(char *str[2], size_t len, char *limit, char *warn)
{
	char				*temp;

	while (!str[0] || ft_strncmp(str[0], limit, len) || ft_strlen(limit) != len)
	{
		temp = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		free(temp);
		free(str[0]);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		str[0] = readline("> ");
		signal(SIGINT, handle_sigint_child);
		if (!str[0])
		{
			printf("%s (wanted `%s\')\n", warn, limit);
			break ;
		}
		temp = str[0];
		str[0] = ft_strjoin(str[0], "\n");
		free(temp);
		len = ft_strlen(str[0]) - 1;
	}
	free(str[0]);
	return (minishell_here_fd(str[1]));
}
