/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:50 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/10 17:03:58 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/inc/libft.h"
# include "colors.h"
# include "get_next_line.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

# define READ_END 0
# define WRITE_END 1

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
}			t_prompt;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_mini;

enum	e_mini_error
{
	QUOTE = 1,
	NDIR = 2,
	PWD = 3,
	NCMD = 4,
};

/* Handles all builtin functions */
int		builtin(t_prompt *prompt);

/* C implementation of the cd shell command */
int		mini_cd(t_prompt *prompt);

/* C implementation of the pwd shell command */
int		mini_pwd(t_prompt *prompt);

/* C implementation of the echo shell command */
int		mini_echo(t_prompt *prompt);

/* C implementation of the env shell command */
int		mini_env(t_prompt *prompt);

/* C implementation of the export shell command */
int		mini_export(t_prompt *prompt);

/* C implementation of the unset shell command */
int		mini_unset(t_prompt *prompt);

/* Splits command and args into a matrix, taking quotes into account */
char	**ft_cmdtrim(char const *s, char *set);

/* Strtrim from all occurrences of set */
char	*ft_strtrim_all(char const *s1, int squote, int dquote);

/* Parses all necessary stuff for a command matrix */
t_mini	*fill_node(char **args, t_mini *node);

/* Opens a file to a file descriptor with the adequate open flags */
int		get_fd(int oldfd, char *path, int is_outfile, int append);

/* Tries to open proper file as outfile (> case) */
t_mini	*get_outfile1(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open proper file as outfile (>> case) */
t_mini	*get_outfile2(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open proper file as infile (< case) */
t_mini	*get_infile1(t_mini *node, char **args, char **arg, int ij[2]);

/* Tries to open read heredoc as infile (<< case) */
t_mini	*get_infile2(t_mini *node, char **args, char **arg, int ij[2]);

/* Fills in linked list node with command info */
t_list	*parse_args(char **args, t_prompt *prompt);

/* Executes a non-builtin command according to the info on our list */
int		exec_cmd(t_list *cmd, char **envp);

/* Executes a custom command and saves output to string ending in \n */
void	exec_custom(char ***out, char *full, char *args, char **envp);

/* Executes a non-builtin command according to the info on our list */
int		exec_builtin(t_prompt *prompt, int (*func)(t_prompt *));

/* Checks if a command is in the PATH variable and retrieves the full_path */
void	get_cmd(t_mini *node);

/* Expands environment variables in a string if not in quotes */
char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt);

/* Expands "~" to home directory in a string if not in quotes */
char	*expand_path(char *str, int i, int quotes[2], char *var);

/* Retrieves a string from standard input, expanding vars when needed */
int		get_here_doc(char *str, char *full, char *limit, char *warn);

/* Prints a custom error message to standard error */
int		mini_perror(int err, char *param);

/* Retrieves a string with malloc containing the value of an env var */
char	*mini_getenv(char	*var, char **envp, int n);

/* Sets a new environment variable */
void	*mini_setenv(char *var, char *value, char **envp, int n);

/* Returns a colorized string used as prompt for readline */
char	*mini_getprompt(t_prompt prompt);

#endif
