/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 10:17:00 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/19 13:43:21 by mbueno-g         ###   ########.fr       */
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

int	get_here_doc(char *str, char *full, char *limit, char *warn)
{
	char				*temp;
	size_t				len;

	len = 0;
	while (!str || ft_strncmp(str, limit, len) || ft_strlen(limit) != len)
	{
		temp = full;
		full = ft_strjoin(full, str);
		free(temp);
		free(str);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		str = readline("> ");
		signal(SIGINT, handle_sigint_child);
		if (!str)
		{
			printf("%s (wanted `%s\')\n", warn, limit);
			break ;
		}
		temp = str;
		str = ft_strjoin(str, "\n");
		free(temp);
		len = ft_strlen(str) - 1;
	}
	free(str);
	return (minishell_here_fd(full));
}
