/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:50 by aperez-b          #+#    #+#             */
/*   Updated: 2021/12/31 16:29:03 by aperez-b         ###   ########.fr       */
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
# include <dirent.h>

# define READ_END 0
# define WRITE_END 1

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
	int		e_status;
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
	NPERM = 3,
	NCMD = 6,
	DUPERR = 7,
	FORKERR = 8,
	PIPERR = 9,
	PIPENDERR = 10,
	MEM = 11,
	IS_DIR = 12,
	NOT_DIR = 13
};

/* Uses readline inside a child process and returns the read line */
char	*mini_readline(t_prompt *prompt, char *str);

/* Opens two sets of pipes and checks if they are opened correctly */
void	*mini_here_fd(t_prompt *prompt, int fd[2], int auxfd[2]);

/* Handles all builtin functions */
int		builtin(t_prompt *prompt, t_list *cmd, int *is_exit, int n);

/* Checks if the first element in full_cmd is a builtin */
int		is_builtin(t_mini *n);

/* C implementation of the cd shell command */
int		mini_cd(t_prompt *prompt);

/* Checks for errors and returns whether first arg is a directory */
DIR		*cd_error(t_prompt *prompt, char **str[2]);

/* C implementation of the pwd shell command */
int		mini_pwd(void);

/* C implementation of the echo shell command */
int		mini_echo(t_list *cmd);

/* C implementation of the export shell command */
int		mini_export(t_prompt *prompt);

/* C implementation of the unset shell command */
int		mini_unset(t_prompt *prompt);

/* C implementation of the exit shell command */
int		mini_exit(t_list *cmd, int *is_exit);

/* Splits command string into manageable matrix to store & exec commands */
void	*check_args(char *out, t_prompt *p);

/* Splits command and args into a matrix, taking quotes into account */
char	**ft_cmdtrim(char const *s, char *set);

/* Copy of ft_split but includes separators and takes quotes into account */
char	**ft_cmdsubsplit(char const *s, char *set);

/* Strtrim from all needed quotes in s1 */
char	*ft_strtrim_all(char const *s1, int squote, int dquote);

/* Parses all necessary stuff for a command matrix */
t_list	*fill_nodes(t_prompt *prompt, char **args, int i);

/* Opens a file to a file descriptor with the adequate open flags */
int		get_fd(t_prompt *prompt, int oldfd, char *path, int flags[2]);

/* Tries to open proper file as outfile (> case) */
t_mini	*get_outfile1(t_prompt *prompt, t_mini *node, char **args, int *i);

/* Tries to open proper file as outfile (>> case) */
t_mini	*get_outfile2(t_prompt *prompt, t_mini *node, char **args, int *i);

/* Tries to open proper file as infile (< case) */
t_mini	*get_infile1(t_prompt *prompt, t_mini *node, char **args, int *i);

/* Tries to open read heredoc as infile (<< case) */
t_mini	*get_infile2(t_prompt *prompt, t_mini *node, char **args, int *i);

/* Executes a non-builtin command according to the info on our list */
void	*exec_cmd(t_prompt *prompt, t_list *cmd);

/* Checks if conditions are met to perform a fork */
void	*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2]);

/* Execute commands inside child process */
void	child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd);

/* Executes a custom command and saves output to string ending in \n */
void	exec_custom(char ***out, char *full, char *args, char **envp);

/* Executes a non-builtin command according to the info on our list */
int		exec_builtin(t_prompt *prompt, int (*func)(t_prompt *));

/* Checks if a command is in the PATH variable and retrieves the full_path */
void	get_cmd(t_prompt *prompt, t_list *start, char **split_path, char *path);

/* Expands environment variables in a string if not in quotes */
char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt);

/* Expands "~" to home directory in a string if not in quotes */
char	*expand_path(char *str, int i, int quotes[2], char *var);

/* Retrieves a string from standard input, expanding vars when needed */
int		get_here_doc(t_prompt *prompt, char *str[2], size_t len, char *aux[2]);

/* Prints a custom error message to standard error */
void	*mini_perror(t_prompt *prompt, int err_type, char *param, int errno);

/* Retrieves a string with malloc containing the value of an env var */
char	*mini_getenv(char	*var, char **envp, int n);

/* Sets a new environment variable */
char	**mini_setenv(char *var, char *value, char **envp, int n);

/* Returns a colorized string used as prompt for readline */
char	*mini_getprompt(t_prompt prompt);

/* Frees all elements in linked list nodes */
void	free_content(void *content);

/* Function to handle SIGINT signals for main process */
void	handle_sigint(int sig);

/* Function to handle SIGINT signals for child process */
void	handle_sigint_child(int sig);

/* Function to handle SIGQUIT signals for child process */
void	handle_sigquit(int sig);

#endif
