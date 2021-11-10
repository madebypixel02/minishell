/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 10:59:42 by aperez-b          #+#    #+#             */
/*   Updated: 2021/09/05 20:25:10 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE

#  define BUFFER_SIZE 42

# endif

/* Retrieves next line from given file descriptor */
char	*get_next_line(int fd);

/* Returns length of a string */
size_t	gnl_strlen(const char *s);

/* Retrieves index of first appearance of i in s, or -1 */
int		gnl_strchr_i(const char *s, int c);

/* Copies chars from src to dst ensuring null-termination */
size_t	gnl_strlcpy(char *dst, const char *src, size_t size);

/* Allocates enough memory to make a copy of s from start, copying len bytes */
char	*gnl_substr(char const *s, unsigned int start, size_t len);

/* Concatenates two strings, ensuring null-termination */
size_t	gnl_strlcat(char *dst, const char *src, size_t size);

/* Removes line from the static variable buf */
char	*gnl_shrink_buffer(char *buf, char *line);

/* Performs a new read and adds it to static variable buf */
char	*gnl_expand_buffer(char *buf, int fd);

/* Performs a new read and returns read string */
char	*gnl_newread(int fd);

#endif
