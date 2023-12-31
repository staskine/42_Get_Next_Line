/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sataskin <sataskin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:28:11 by sataskin          #+#    #+#             */
/*   Updated: 2023/12/28 14:40:14 by sataskin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*This function frees an array and returns a NULL for error handling */

char	*ft_ret_strdel(char **string)
{
	free(*string);
	*string = NULL;
	return (NULL);
}

/* This function takes the static variable and returns the line from it.
 * If the file ends it just duplicates the string and frees it. Otherwise 
 * it extracts what it needed and leaves the rest */

static char	*get_result(char **string)
{
	size_t		i;
	char		*temp;
	char		*line;

	i = 0;
	if (string[0][i] == '\0')
		return (ft_ret_strdel(string));
	while (string[0][i] != '\0' && string[0][i] != '\n')
		i++;
	if (string[0][i] == '\n' && string[0][i + 1] != '\0')
	{
		line = ft_substr(string[0], 0, i + 1);
		temp = *string;
		*string = ft_substr(temp, (i + 1), (ft_strlen(temp) - i));
		free(temp);
		if (!(*string) || !line)
		{
			free(line);
			return (ft_ret_strdel(string));
		}
		return (line);
	}
	line = ft_strdup(*string);
	ft_ret_strdel(string);
	return (line);
}

/* This function compiles the string and the buffer from read */

static char	*ft_strcompile(char **string, char *buf)
{
	char	*temp;

	if (*string == NULL)
	{
		*string = ft_strdup(buf);
		if (!(*string))
			return (NULL);
	}
	else
	{
		temp = *string;
		*string = ft_strjoin(temp, buf);
		free(temp);
		if (!(*string))
			return (NULL);
	}
	return (*string);
}

/*This function checks if there was an error with read. Otherwise 
 * it just continues. */

static char	*check_result(char **string, int ret)
{
	if (ret < 0)
		return (ft_ret_strdel(string));
	return (get_result(string));
}

/* This is the main function. It first checks for error handling. 
 * Then it checks if the static variable has a line in it. If not it continues 
 * to read the file BUFFER_SIZE at a time. It continues this until either the 
 * file ends or there is a full line. */

char	*get_next_line(int fd)
{
	static char	*string;
	char		buf[BUFFER_SIZE + 1];
	int			ret;

	if (fd < 0 || fd > FD_SIZE || read(fd, buf, 0) < 0)
		return (ft_ret_strdel(&string));
	if (string != 0)
	{
		if (ft_strchr(string, '\n'))
			return (get_result(&string));
	}
	ret = read(fd, buf, BUFFER_SIZE);
	if ((ret == 0 && !string) || ret < 0)
		return (ft_ret_strdel(&string));
	while (ret > 0)
	{
		buf[ret] = '\0';
		string = ft_strcompile(&string, buf);
		if (!string)
			return (ft_ret_strdel(&string));
		if (ft_strchr(string, '\n'))
			break ;
		ret = read(fd, buf, BUFFER_SIZE);
	}
	return (check_result(&string, ret));
}
