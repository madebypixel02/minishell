/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <aperez-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:02:33 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/12 09:28:54 by aperez-b         ###   ########.fr       */
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
	if (user && !ft_strncmp(*user, "root", 4))
		temp2 = ft_strjoin(NULL, RED);
	else if (user && (int)(*user) % 5 == 0)
		temp2 = ft_strjoin(NULL, GREEN);
	else if (user && (int)(*user) % 5 == 1)
		temp2 = ft_strjoin(NULL, GRAY);
	else if (user && (int)(*user) % 5 == 2)
		temp2 = ft_strjoin(NULL, YELLOW);
	else if (user && (int)(*user) % 5 == 3)
		temp2 = ft_strjoin(NULL, MAGENTA);
	else if (user && (int)(*user) % 5 == 4)
		temp2 = ft_strjoin(NULL, CYAN);
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
	temp2 = ft_strjoin(temp, DEFAULT);
	free(temp);
	temp = ft_strjoin(temp2, "$ ");
	free(temp2);
	temp2 = ft_strjoin(temp, DEFAULT);
	free(temp);
	return (temp2);
}
