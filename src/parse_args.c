/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/10/29 18:52:02 by mbueno-g         ###   ########.fr       */
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

static char	*expand_vars(char *str, int i)
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
		if (str[i] == '\"' && str[i + 1] == '$')
		{
			j = ft_strchr_i(&str[i + 2], '\"');
			var = ft_substr(&str[i + 2], 0, j);
			if (!var)
				return (NULL);
			exp1  = getenv(var);
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
	ft_strlcat(ptr, str, i + 2);
	ft_strlcat(ptr, exp1, len + 1);
	len2 = ft_strlen(ptr);
	ft_strlcat(ptr, &str[i + ft_strlen(var) + 2], len + 1); 
	return (expand_vars(ptr, len2));
}

static char	**expand_matrix(char ***args)
{
	char	**aux;
	int		i;

	i = 0;
	aux = malloc(sizeof(char *) * (ft_matrixlen(*args) + 1));
	if (!aux)
		return (NULL);
	aux[ft_matrixlen(*args)] = NULL;
	while (args[0][i])
	{
		aux[i] = expand_vars(args[0][i], 0);
		if (!aux[i])
		{
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
	char	**str = ft_split("echo \"$PATH\" ggg \"$HOLA\" EEE", ' ');
	ft_putmatrix_fd(expand_matrix(&str), 1);
	//char	*ptr = "echo  \"$PWD\" ggg \"$HOLA\" EEE";
	//printf("%s\n", expand_vars(ptr, 0));
}

