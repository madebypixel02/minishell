/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 11:36:47 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/12 00:11:46 by aperez-b         ###   ########.fr       */
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
		ft_putstr_fd("minishell: pwd: too many arguments\n", 2);
	else if (err == NCMD)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(param, 2);
	}
	return (1);
}

void	free_content(void *content)
{
	t_mini	*node;

	node = content;
	ft_free_matrix(&node->full_cmd);
	free(node->full_path);
	free(node);
}

/*void	*mini_exit(t_prompt *prompt, int err)
{
	
}*/
