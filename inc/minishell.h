/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:50 by aperez-b          #+#    #+#             */
/*   Updated: 2021/10/29 12:04:14 by mbueno-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

typedef struct	s_mini
{
	char	*cmd;
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}				t_mini;

/* C implementation of the cd shell command */
int	cd(char **argv);

/* Handles all builtin functions */
int	builtin(int argc, char **argv, char **envp);

/* C implementation of the pwd shell command */
int	pwd(int argc);

/* C implementation of the echo shell command */
int	echo(char **argv);	

/* C implementation of the env shell command */
int		env(int argc, char **envp);

/* Splits command and args into a matrix, taking quotes into account */
char	**ft_cmdtrim(char const *s, char c);

#endif
