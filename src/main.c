/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/22 16:46:03 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	char	*out;
	char	**args;

	out = NULL;
	str = "minishell$ ";
	while (1)
	{
		out = readline(str);
		args = ft_split(out, ' ');
		free(out);
		if (builtin(ft_matrixlen(args), args, envp))
		{
			ft_free_matrix(&args);
			return (printf("exit\n"));
		}
		ft_free_matrix(&args);
	}
}
