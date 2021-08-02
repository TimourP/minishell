/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:57:39 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/02 15:07:23 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int words_count(char *str, char c)
{
	int i;
	int count;

	count = 0;
	i = 0;
	if (str == NULL)
		return (0);
	while (str[++i])
	{
		if (str[i - 1] != c && str[i] == c)
			count++;
	}
	if (str[i - 1] && str[i - 1] != c)
		count++;
	return (count);
	
}

char **parse_split(char *str, char c)
{
	const int w_count = words_count(str, c);
	const int str_len = my_strlen(str);
	char **split_list;
	int i;
	int current_word;

	current_word = 0;
	i = -1;
	split_list = malloc(sizeof(char *) * (w_count + 1));
	split_list[w_count] = NULL;
	while (str[++i])
		if (str[i] == c)
			str[i] = 0;
	i = -1;
	while (++i < str_len)
	{
		if (str[i] == 0)
			;
		else
		{
			split_list[current_word] = my_strdup(&str[i]);
			i += my_strlen(split_list[current_word]) - 1;
			current_word++;
		}
	}
	return (split_list);
}

/*
** get_cmd_from_line get first word of str
*/

static char *get_cmd_from_line(char *str)
{
	int i;
	int last;
	int len;
	char *ret;

	i = -1;
	len = 0;
	last = 0;
	while (str[++i])
	{
		if (str[i] == ' ')
		{
			if (len != 0)
			{
				str[i] = 0;
				break;
			}
			else
				last = i + 1;
		}
		else
			len++;
	}
	ret = my_strdup(&str[last]);
	str[i] = ' ';
	return (ret);
}

/*
** my_strip remove char c from start and end of str. This function does not free str.
*/

char *my_strip(char *str, char c)
{
	int i;
	int start;
	int end;
	const int len = my_strlen(str);
	char *ret;

	i = -1;
	start = 0;
	end = len;
	while (str[++i] && str[i] == c)
		start = i + 1;
	i = -1;
	while (++i < len && str[len - 1 - i] == c)
		end = len - 1 - i;
	str[end] = 0;
	ret = my_strdup(&str[start]);
	return (ret);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	while (1)
	{
		if (!*s1 || !*s2)
			return (0);
		if (*s1 == *s2)
		{
			s1++;
			s2++;
		}
		else
			return ((int)((unsigned char)(*s1) - (unsigned char)(*s2)));
	}
}


char *get_env_value(char **env, char *var)
{
	int i;

	i = -1;
	while (env[++i])
	{
		if (my_strlen(var) < my_strlen(env[i]) && ft_strcmp(env[i], var) == 0 && env[i][my_strlen(var)] == '=')
			return my_strdup(&env[i][my_strlen(var) + 1]);
	}
	return my_strdup("");
}

int	parse_line(t_shell *shell, char *line)
{
	char **split_line;
	char *strip;
	int i;
	t_cmd *new;

	i = -1;
	cmd_clear(&shell->start_cmd);
	shell->start_cmd = NULL;
	split_line = parse_split(line, '|');
	printf("getenv: %s\n", get_env_value(shell->env, line));
	while (split_line[++i] != NULL)
	{
		strip = my_strip(split_line[i], ' ');
		new = cmd_new(get_cmd_from_line(split_line[i]), parse_split(strip, ' '));
		cmd_add_back(&shell->start_cmd, new);
		free(strip);
		free(split_line[i]);
	}
	free(split_line);
	return (0);
}