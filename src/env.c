/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 17:28:02 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/09 19:37:45 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	var_in_envp(char *argv, char **envp, int ij[2])
{
	int	pos;

	ij[1] = 0;
	pos = ft_strchr_i(argv, '=');
	if (pos == -1)
		return (-1);
	while (envp[ij[1]])
	{
		if (!ft_strncmp(envp[ij[1]], argv, pos))
			return (1);
		ij[1]++;
	}
	return (0);
}

int	export(t_prompt *prompt, int argc, char **argv)
{
	int		ij[2];
	int		pos;

	if (argc >= 2)
	{
		ij[0] = 1;
		while (argv[ij[0]])
		{
			pos = var_in_envp(argv[ij[0]], prompt->envp, ij);
			if (pos)
			{
				free(prompt->envp[ij[1]]);
				prompt->envp[ij[1]] = ft_strdup(argv[ij[0]]);
			}
			else if (!pos)
			{
				printf("holaa\n");
				prompt->envp = ft_extend_matrix(prompt->envp, argv[ij[0]]);
			}
			ij[0]++;
		}
	}
	ft_putmatrix_fd(prompt->envp, 1);
	//ft_free_matrix(&aux);
	return (1);
}
