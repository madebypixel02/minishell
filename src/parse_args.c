/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/03 10:12:10 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <strings.h>

/*static t_mini	*fill_node(char *in, char *out, char *cmd)
{
	t_mini *m;

	m = malloc(sizeof(t_mini));
	if (!m)
		return (NULL);
	m->cmd =
	m->full_cmd = 
	m->full_path =
	m->infile = 
	m->outfile = 
}*/

static char	*get_substr(char *str, int len[3], int ij[2], int quotes[2])
{
	char	*var;
	char	*exp1;

	ij[0] = -1;
	exp1 = NULL;
	while (++ij[0] >= 0 && str && str[ij[0]])
	{
		quotes[0] = (quotes[0] + (str[ij[0]] == '\'')) % 2;
		quotes[1] = (quotes[1] + (str[ij[0]] == '\"')) % 2;
		if ((!quotes[0] || quotes[1]) && str[ij[0]] == '$' && \
			ft_strchars_i(&str[ij[0] + 1], "\"\' $"))
		{
			ij[1] = ft_strchars_i(&str[ij[0] + 1], "\"\' $");
			var = ft_substr(&str[ij[0] + 1], 0, ij[1]);
			exp1 = getenv(var);
			if (!exp1)
				exp1 = "";
			len[0] = len[0] - ft_strlen(var) - 1 + ft_strlen(exp1);
			len[2] = ft_strlen(var);
			free(var);
			break ;
		}
	}
	return (exp1);
}

static char	*expand_vars(char *str, int ij[2], int quotes[2])
{
	int		len[3];
	char	*exp1;
	char	*ptr;

	quotes[0] = 0;
	quotes[1] = 0;
	ij[0] = -1;
	ij[1] = -2;
	len[0] = ft_strlen(str);
	len[2] = 0;
	exp1 = get_substr(str, len, ij, quotes);
	if (ij[1] == -2)
		return (str);
	ptr = ft_calloc(sizeof(char), (len[0] + 1));
	if (!ptr)
		return (NULL);
	ft_strlcat(ptr, str, ij[0] + 1);
	ft_strlcat(ptr, exp1, len[0] + 1);
	len[1] = ft_strlen(ptr);
	ft_strlcat(ptr, &str[ij[0] + len[2] + 1], len[0] + 1);
	free(str);
	ij[1] = len[1];
	return (expand_vars(ptr, ij, quotes));
}

static char	**expand_matrix(char ***args)
{
	char	**aux;
	char	*str;
	int		i;
	int		quotes[2];
	int		ij[2];

	i = -1;
	aux = ft_calloc(sizeof(char *), (ft_matrixlen(*args) + 1));
	if (!aux)
		return (NULL);
	while (++i >= 0 && args[0][i])
	{
		str = expand_vars(ft_strdup(args[0][i]), ij, quotes);
		aux[i] = ft_strtrim_all(str, 0, 0);
		free(str);
		if (!aux[i])
		{
			printf("minishell: error while looking for matching quote\n");
			ft_free_matrix(args);
			ft_free_matrix(&aux);
			return (NULL);
		}
	}
	ft_free_matrix(args);
	return (aux);
}

/*t_list	*parse_args(char **args)
{
	t_list	*cmds;
	int		i;
	char	*in;
	char	*out;
	char	*cmd;
	int		n;
	char	**

	cmds = NULL;
	i = 0;
	if (!expand_matrix(&args))
		return (NULL);
	while(args[i])
	{
		n = ft_countchar(args[i], '<');
		if (n)
		{
			i++;
			if (n == 1)
				in = args[i];
		}
		i++;
		cmd = args[i]		
		n = ft_countchar(args[i], '>');
		if (n)
		{
			i++;
			if (n == 1)
				out = args[i];
		}
		ft_lstadd_back(&cmds, ft_lstnew(fill_node(in, out, cmd)));
		i++;
	}	
	return (NULL);
}*/

int	main(void)
{
	char	**str;
	char	**matrix;

	str = ft_cmdtrim("echo    \"\'$HOLA\'$PD EE$\"", ' ');
	if (str)
	{
		matrix = expand_matrix(&str);
		ft_putmatrix_fd(matrix, 1);
		ft_free_matrix(&matrix);
	}
}
