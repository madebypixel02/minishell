/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbueno-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 11:57:42 by mbueno-g          #+#    #+#             */
/*   Updated: 2021/11/04 13:43:16 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

static char	**expand_matrix(char ***args, int quotes[2])
{
	char	**aux;
	char	*str;
	int		i;
	int		ij[2];

	i = -1;
	aux = ft_calloc(sizeof(char *), (ft_matrixlen(*args) + 1));
	while (aux && ++i >= 0 && args[0][i])
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

t_list	*parse_args(char **args)
{
	t_list	*cmds;
	t_mini	*node;
	int		quotes[2];

	cmds = NULL;
	args = expand_matrix(&args, quotes);
	if (!args)
		return (NULL);
	node = fill_node(args);
	if (!node)
	{
		ft_lstclear(&cmds, free);
		return (NULL);
	}
	ft_lstadd_back(&cmds, ft_lstnew(node));
	ft_free_matrix(&args);
	return (cmds);
}

int	main(void)
{
	char	*str;
	char	**matrix1;
	t_list	*cmds;

	str = "echo abc>> \"hello\'\"\' >> hola";
	matrix1 = ft_cmdtrim(str, " ");
	if (!matrix1)
		return (0);
	cmds = parse_args(matrix1);
	if (!cmds)
		return (0);
	printf("%d\n", ((t_mini *)cmds->content)->outfile);
	close(((t_mini *)cmds->content)->outfile);
	ft_lstclear(&cmds, free);
}
