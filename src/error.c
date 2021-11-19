/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 11:36:47 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/19 18:35:11 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*mini_perror(t_prompt *prompt, int err, char *param)
{
	prompt->e_status = 1;
	if (err == QUOTE)
		ft_putstr_fd("minishell: error while looking for matching quote\n", 2);
	else if (err == NDIR)
		ft_putstr_fd("minishell: No such file or directory: ", 2);
	else if (err == NPERM)
		ft_putstr_fd("minishell: permission denied: ", 2);
	else if (err == PWD)
		ft_putstr_fd("minishell: pwd: too many arguments\n", 2);
	else if (err == NCMD)
		ft_putstr_fd("minishell: command not found: ", 2);
	else if (err == DUPERR)
		ft_putstr_fd("minishell: dup2 failed\n", 2);
	else if (err == FORKERR)
		ft_putstr_fd("minishell: fork failed\n", 2);
	else if (err == MEM)
		ft_putstr_fd("minishell: no memory left on device\n", 2);
	else if (err == PIPERR)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	if (param)
		ft_putendl_fd(param, 2);
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
