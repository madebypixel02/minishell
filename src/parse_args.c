/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/02 20:01:16 by aperez-b         ###   ########.fr       */
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

static char	*expand_vars(char *str, int i, int squote, int dquote)
{
	int		len;
	int		j;
	char	*var;
	char	*exp1;
	char	*ptr;
	int		len2;


	j = -2;
	len = ft_strlen(str);
	while (str && str[i])
	{
		squote = (squote + (str[i] == '\'')) % 2;
		dquote = (dquote + (str[i] == '\"')) % 2;
		if ((!squote || dquote) && str[i] == '$' && ft_strchars_i(&str[i + 1], "\"\' $"))
		{
			j = ft_strchars_i(&str[i + 1], "\"\' $");
			var = ft_substr(&str[i + 1], 0, j);
			if (!var)
				return (NULL);
			exp1 = getenv(var);
			if (!exp1)
				exp1 = "";
			len = len - ft_strlen(var) - 1 + ft_strlen(exp1);
			break;
		}
		i++;
	}
	if (j == -2)
		return (ft_strdup(str));
	ptr = ft_calloc(sizeof(char), (len + 1));
	if (!ptr)
	{
		free(var);
		return (NULL);
	}
	ptr[len] = '\0';
	ft_strlcat(ptr, str, i + 1);
	ft_strlcat(ptr, exp1, len + 1);
	len2 = ft_strlen(ptr);
	ft_strlcat(ptr, &str[i + ft_strlen(var) + 1], len + 1); 
	return (expand_vars(ptr, len2, squote, dquote));
}

static char	**expand_matrix(char ***args)
{
	char	**aux;
	char	*str;
	int		i;

	i = 0;
	aux = malloc(sizeof(char *) * (ft_matrixlen(*args) + 1));
	if (!aux)
		return (NULL);
	aux[ft_matrixlen(*args)] = NULL;
	while (args[0][i])
	{
		str = expand_vars(args[0][i], 0, 0, 0);
		aux[i] = ft_strtrim_all(str);
		free(str);
		if (!aux[i])
		{
			printf("minishell: unexpected EOF while looking for matching quote\n");
			ft_free_matrix(&aux);
			return (NULL);
		}
		i++;
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

int	main()
{
	char	**str = ft_cmdtrim("\'$P\"W\'D\'\"", ' ');
	ft_putmatrix_fd(expand_matrix(&str), 1);
	//char	*ptr = "echo  \"$PWD\" ggg \"$HOLA\" EEE";
	//printf("%s\n", expand_vars(ptr,  0));
}

