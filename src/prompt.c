/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:02:33 by aperez-b          #+#    #+#             */
/*   Updated: 2022/03/07 18:11:04 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*get_home(t_prompt prompt)
{
	char	*temp;
	char	*pwd;
	char	*home;

	pwd = getcwd(NULL, 0);
	home = mini_getenv("HOME", prompt.envp, 4);
	if (home && home[0] && ft_strnstr(pwd, home, ft_strlen(pwd)))
	{
		temp = pwd;
		pwd = ft_strjoin("~", &pwd[ft_strlen(home)]);
		free(temp);
	}
	free(home);
	home = ft_strjoin(BLUE, pwd);
	free(pwd);
	pwd = ft_strjoin(home, " ");
	free(home);
	home = ft_strjoin(" ", pwd);
	free(pwd);
	pwd = ft_strjoin(home, DEFAULT);
	free(home);
	return (pwd);
}

static char	*get_user(t_prompt prompt)
{
	char	**user;
	char	*temp;
	char	*temp2;

	user = NULL;
	temp2 = NULL;
	exec_custom(&user, "/usr/bin/whoami", "whoami", prompt.envp);
	if (!user)
		user = ft_extend_matrix(user, "guest");
	if (!ft_strncmp(user[0], "root", 4))
		temp2 = ft_strjoin(NULL, RED);
	else if ((int)(user[0][0]) % 5 == 0)
		temp2 = ft_strjoin(NULL, CYAN);
	else if ((int)(user[0][0]) % 5 == 1)
		temp2 = ft_strjoin(NULL, GRAY);
	else if ((int)(user[0][0]) % 5 == 2)
		temp2 = ft_strjoin(NULL, GREEN);
	else if ((int)(user[0][0]) % 5 == 3)
		temp2 = ft_strjoin(NULL, MAGENTA);
	else
		temp2 = ft_strjoin(NULL, YELLOW);
	temp = ft_strjoin(temp2, *user);
	free(temp2);
	ft_free_matrix(&user);
	return (temp);
}

char	*mini_getprompt(t_prompt prompt)
{
	char	*temp;
	char	*temp2;
	char	*aux;

	temp = get_user(prompt);
	temp2 = ft_strjoin(temp, "@minishell");
	free(temp);
	aux = get_home(prompt);
	temp = ft_strjoin(temp2, aux);
	free(aux);
	free(temp2);
	if (!prompt.e_status || prompt.e_status == -1)
		temp2 = ft_strjoin(temp, BLUE);
	else
		temp2 = ft_strjoin(temp, RED);
	free(temp);
	temp = ft_strjoin(temp2, "$ ");
	free(temp2);
	temp2 = ft_strjoin(temp, DEFAULT);
	free(temp);
	return (temp2);
}

void	readline_child(char *str, int fd[2], char *out, int is_null)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(fd[READ_END]);
	out = readline(str);
	write(fd[WRITE_END], out, ft_strlen(out));
	is_null = !out;
	free(out);
	close(fd[WRITE_END]);
	exit(is_null);
}

char	*mini_readline(t_prompt *prompt, char *str)
{
	pid_t	pid;
	int		is_null;
	char	*out;
	int		fd[2];

	is_null = 0;
	out = NULL;
	if (!mini_here_fd(prompt, fd))
		return (NULL);
	pid = fork();
	if (pid == -1)
		mini_perror(prompt, FORKERR, NULL, 1);
	if (!pid)
		readline_child(str, fd, out, is_null);
	close(fd[WRITE_END]);
	waitpid(pid, &is_null, 0);
	out = get_next_line(fd[READ_END]);
	close(fd[READ_END]);
	if (is_null == 33280)
		prompt->e_status = 130;
	if (is_null != 256 && !out)
		out = ft_strdup("");
	return (out);
}
