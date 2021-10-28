/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:50 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/28 15:30:57 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

/* C implementation of the cd shell command */
void	cd(char **argv);

/* Handles all builtin functions */
int		builtin(int argc, char **argv, char **envp);

/* C implementation of the pwd shell command */
int		pwd(int argc);

/* C implementation of the env shell command */
int		env(int argc, char **envp);

/* Splits command and args into a matrix, ignoring quotes and expanding vars */
char	**ft_cmdtrim(char const *s, char c);

#endif
