/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 11:36:47 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/13 19:24:10 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*mini_perror(int err, char *param)
{
	if (err == QUOTE)
		ft_putstr_fd("minishell: error while looking for matching quote\n", 2);
	else if (err == NDIR)
	{
		ft_putstr_fd("minishell: cd: No such file or directory: ", 2);
		ft_putendl_fd(param, 2);
	}
	else if (err == PWD)
		ft_putstr_fd("minishell: pwd: too many arguments\n", 2);
	else if (err == NCMD)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(param, 2);
	}
	else if (err == CMDERR)
		ft_putstr_fd("minishell: execve failed\n", 2);
	else if (err == DUPERR)
		ft_putstr_fd("minishell: dup2 failed\n", 2);
	else if (err == FORKERR)
		ft_putstr_fd("minishell: fork failed\n", 2);
	else if (err == MEM)
		ft_putstr_fd("minishell: no memory left on device\n", 2);
	else if (err == PIPERR)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	return (NULL);
}

void	free_content(void *content)
{
	t_mini	*node;

	node = content;
	ft_free_matrix(&node->full_cmd);
	free(node->full_path);
	if (node->infile != STDIN_FILENO)
		close(node->infile);
	if (node->outfile != STDOUT_FILENO)
		close(node->outfile);
	free(node);
}

/*void	*mini_exit(t_prompt *prompt, int err)
{
	
}*/
