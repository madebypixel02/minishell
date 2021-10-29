/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/10/29 17:22:50 by mbueno-g         ###   ########.fr       */
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

static char	*expand_vars(char *str)
{
	int		len;
	int		i;
	int		j;
	char	*var;
	char	*exp1;
	char	*ptr;

	i = 0;
	j = -2;
	len = ft_strlen(str);
	printf("str %s\n", str);
	while (str && str[i])
	{
		if (str[i] == '\"' && str[i + 1] == '$')
		{
			printf("holaaa222 %d\n", i);
			j = ft_strchr_i(&str[i + 2], '\"');
			var = ft_substr(&str[i + 2], 0, j);
			printf("var %s\n", var);
			if (!var)
				return (NULL);
			exp1  = getenv(var);
			printf("exp1 %s\n", exp1);
			len = len - ft_strlen(var) - 1 + ft_strlen(exp1);
			break;
		}
		i++;
	}
	printf("ii %d\n", i);
	if (j == -2)
		return (ft_strdup(str));
	printf("hlaaaaaaaaaaaa\n");
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
	{
		free(var);
		return (NULL);
	}
	ptr[len] = '\0';
	strlcat(ptr, str, i + 2);
	strlcat(ptr, exp1, len + 1);
	strlcat(ptr, &str[i + ft_strlen(var) + 2], len + 1); 
	free(str);
	return (expand_vars(ptr));
}

/*static char	**expand_matrix(char ***args)
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
		aux[i] = expand_vars(args[0][i]);
		if (!aux[i])
		{
			ft_free_matrix(&aux);
			return (NULL);
		}
		i++;
	}
	ft_free_matrix(args);
	args = &aux;
	return (args);
}*/

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
	char	*str = strdup("echo \"$PWD\" ggg \"$PWD\" EEE");
	printf("%s\n", expand_vars(str));
}

