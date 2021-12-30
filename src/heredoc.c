/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 10:17:00 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/12/30 13:49:57 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int	g_fds[2][2];

void	*mini_here_fd(t_prompt *prompt, int fd[2], int auxfd[2])
{
	if (pipe(fd) == -1)
		return (mini_perror(prompt, PIPERR, NULL, 1));
	if (pipe(auxfd) == -1)
	{
		close(fd[READ_END]);
		close(fd[WRITE_END]);
		return (mini_perror(prompt, PIPERR, NULL, 1));
	}
	return ("");
}

char	*get_here_str(char *str[2], size_t len, char *limit, char *warn)
{
	char	*temp;

	while (!str[0] || ft_strncmp(str[0], limit, len) || ft_strlen(limit) != len)
	{
		temp = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		free(temp);
		free(str[0]);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		str[0] = readline("> ");
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
	return (str[1]);
}

void	here_child(t_prompt *prompt, char *str[2], size_t len, char *aux[2])
{
	close(g_fds[0][READ_END]);
	close(g_fds[1][READ_END]);
	str[1] = get_here_str(str, len, aux[0], aux[1]);
	write(g_fds[0][WRITE_END], str[1], ft_strlen(str[1]));
	free(str[1]);
	prompt->e_status = 0;
	write(g_fds[1][WRITE_END], &prompt->e_status, sizeof(int));
	close(g_fds[1][WRITE_END]);
	close(g_fds[0][WRITE_END]);
	exit(0);
}

int	get_here_doc(t_prompt *prompt, char *str[2], size_t len, char *aux[2])
{
	pid_t	pid;

	if (!mini_here_fd(prompt, g_fds[0], g_fds[1]))
		return (-1);
	pid = fork();
	if (!pid)
		here_child(prompt, str, len, aux);
	close(g_fds[0][WRITE_END]);
	close(g_fds[1][WRITE_END]);
	waitpid(pid, NULL, 0);
	read(g_fds[1][READ_END], &prompt->e_status, sizeof(int));
	close(g_fds[1][READ_END]);
	if (prompt->e_status == 130)
	{
		close(g_fds[0][READ_END]);
		return (-1);
	}
	return (g_fds[0][READ_END]);
}
