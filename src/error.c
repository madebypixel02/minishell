/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 11:36:47 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/09 12:11:47 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	mini_perror(int err, char *param)
{
	if (err == QUOTE)
		ft_putstr_fd("minishell: error while looking for matching quote\n", 2);
	else if (err == NDIR)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(param, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else if (err == PWD)
		ft_putstr_fd("pwd: too many arguments\n", 2);
	else if (err == NCMD)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(param, 2);
	}
	else if (err == WEXIT)
		ft_putstr_fd("exit\n", 2);
	else if (err == REXIT)
		ft_putstr_fd("exit\n", 1);
	return (1);
}
