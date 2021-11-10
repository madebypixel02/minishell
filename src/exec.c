/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2021/11/10 19:17:04 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	find_command(char **env_path, char *cmd, char **full_path)
{
	char	*temp;
	int		i;

	i = -1;
	*full_path = NULL;
	while (env_path && env_path[++i])
	{
		free(*full_path);
		temp = ft_strjoin(env_path[i], "/");
		if (!temp)
			return (-2);
		*full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (-2);
		if (access(*full_path, F_OK) == 0)
			break ;
	}
	if (!env_path || !env_path[i])
	{
		free(*full_path);
		*full_path = NULL;
		return (-1);
	}
	return (0);
}

void	get_cmd(t_prompt *prompt)
{
	char	**split_path;
	char	*path;
	t_mini	*node;

	node = prompt->cmds->content;
	if (ft_strchr(*node->full_cmd, '/') && !access(*node->full_cmd, F_OK))
	{
		split_path = ft_split(*node->full_cmd, '/');
		node->full_path = ft_strdup(*node->full_cmd);
		free(node->full_cmd[0]);
		node->full_cmd[0] = ft_strdup(split_path[ft_matrixlen(split_path) - 1]);
	}
	else
	{
		path = mini_getenv("PATH", prompt->envp, 4);
		split_path = ft_split(path, ':');
		free(path);
		find_command(split_path, *node->full_cmd, &node->full_path);
		if (!node->full_path)
			mini_perror(NCMD, *node->full_cmd);
	}
	ft_free_matrix(&split_path);
}

int	exec_cmd(t_list *cmd, char **envp)
{
	pid_t	pid;
	t_mini	*node;

	node = cmd->content;
	if (!node->full_path)
		return (0);
	pid = fork();
	if (!pid)
	{
		if (node->infile != STDIN_FILENO)
		{
			dup2(node->infile, STDIN_FILENO);
			close(node->infile);
		}
		if (node->outfile != STDOUT_FILENO)
		{
			dup2(node->outfile, STDOUT_FILENO);
			close(node->outfile);
		}
		execve(node->full_path, node->full_cmd, envp);
		exit(1);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}

static void	update_output(char ***matrix, int fd)
{
	char	**aux;
	char	*temp;
	char	*line;

	aux = NULL;
	line = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		temp = ft_strtrim(line, "\n");
		free(line);
		aux = ft_extend_matrix(aux, temp);
		free(temp);
	}
	ft_free_matrix(matrix);
	*matrix = aux;
}

void	exec_custom(char ***out, char *full, char *args, char **envp)
{
	pid_t	pid;
	int		fd[2];
	char	**matrix;

	pipe(fd);
	pid = fork();
	if (!pid)
	{
		matrix = ft_split(args, ' ');
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		execve(full, matrix, envp);
		ft_free_matrix(&matrix);
		free(full);
		ft_free_matrix(&envp);
		exit (1);
	}
	close(fd[WRITE_END]);
	waitpid(pid, NULL, 0);
	update_output(out, fd[READ_END]);
	close(fd[READ_END]);
}

/*int	exec_builtin(t_prompt *prompt, int (*func)(t_prompt *))
{
	pid_t	pid;
	t_mini	*node;
	int		out;
	int		fd[2];

	node = prompt->cmds->content;
	pipe(fd);
	pid = fork();
	if (!pid)
	{
		close(fd[READ_END]);
		if (node->infile != STDIN_FILENO)
		{
			dup2(node->infile, STDIN_FILENO);
			close(node->infile);
		}
		if (node->outfile != STDOUT_FILENO)
		{
			dup2(node->outfile, STDOUT_FILENO);
			close(node->outfile);
		}
		out = func(prompt);
		ft_putmatrix_fd(prompt->envp, fd[WRITE_END]);
		close(fd[WRITE_END]);
		if (!out)
			exit(0);
		exit (1);
	}
	else
	{
		close(fd[WRITE_END]);
		waitpid(pid, NULL, 0);
		update_envp(&prompt->envp, fd[READ_END]);
		close(fd[READ_END]);
	}
	return (0);
}*/
