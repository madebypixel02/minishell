/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:50 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/04 20:08:48 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

typedef struct s_mini
{
	char	*cmd;
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_mini;

/* C implementation of the cd shell command */
int		cd(char **argv);

/* Handles all builtin functions */
int		builtin(int argc, char **argv, char **envp);

/* C implementation of the pwd shell command */
int		pwd(int argc);

/* C implementation of the echo shell command */
int		echo(char **argv);

/* C implementation of the env shell command */
int		env(int argc, char **envp);

/* Splits command and args into a matrix, taking quotes into account */
char	**ft_cmdtrim(char const *s, char *set);

/* Strtrim from all occurrences of set */
char	*ft_strtrim_all(char const *s1, int squote, int dquote);

/* Parses all necessary stuff for a command matrix */
t_mini	*fill_node(char **args, t_mini *node);

/* Opens a file to a file descriptor with the adequate open flags */
int		get_fd(int oldfd, char *path, int is_outfile, int append);

/* Checks for redirections and fills a node with all needed info */
t_mini	*get_params(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open proper file as outfile (> case) */
t_mini	*get_outfile1(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open proper file as outfile (>> case) */
t_mini	*get_outfile2(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open proper file as infile (< case) */
t_mini	*get_infile1(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open read heredoc as infile (<< case) */
t_mini	*get_infile2(t_mini *node, char **args, char **arg, int ij[2]);

#endif
