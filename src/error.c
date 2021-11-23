/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 11:36:47 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/23 17:08:29 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	*mini_perror(t_prompt *prompt, int err, char *param)
{
	prompt->e_status = 1;
	if (err == PIPENDERR)
		prompt->e_status = 2;
	if (err == QUOTE)
		ft_putstr_fd("minishell: error while looking for matching quote\n", 2);
	else if (err == NDIR)
		ft_putstr_fd("minishell: No such file or directory: ", 2);
	else if (err == NPERM)
		ft_putstr_fd("minishell: permission denied: ", 2);
	else if (err == NCMD)
		ft_putstr_fd("minishell: command not found: ", 2);
	else if (err == DUPERR)
		ft_putstr_fd("minishell: dup2 failed\n", 2);
	else if (err == FORKERR)
		ft_putstr_fd("minishell: fork failed\n", 2);
	else if (err == PIPERR)
		ft_putstr_fd("minishell: error creating pipe\n", 2);
	else if (err == PIPENDERR)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	else if (err == MEM)
		ft_putstr_fd("minishell: no memory left on device\n", 2);
	ft_putendl_fd(param, 2);
	return (NULL);
}

int	ft_atoi2(const char *nptr, long *nbr)
{
	int		sign;

	sign = 1;
	*nbr = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
		sign = -sign;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	if (!ft_isdigit(*nptr))
		return (-1);
	while (ft_isdigit(*nptr))
	{
		*nbr = 10 * *nbr + (*nptr - '0');
		nptr++;
	}
	if (*nptr && !ft_isspace(*nptr))
		return (-1);
	*nbr *= sign;
	return (0);
}

int	mini_exit(t_list *cmd, int *is_exit)
{
	t_mini	*node;
	long	status[2];

	node = cmd->content;
	*is_exit = !cmd->next;
	if (*is_exit)
		ft_putstr_fd("exit\n", 2);
	if (!node->full_cmd || !node->full_cmd[1])
		return (0);
	status[1] = ft_atoi2(node->full_cmd[1], &status[0]);
	if (status[1] == -1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(node->full_cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (255);
	}
	else if (node->full_cmd[2])
	{
		*is_exit = 0;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	status[0] %= 256 + 256 * (status[0] < 0);
	return (status[0]);
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
