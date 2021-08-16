/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multifiles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 10:58:09 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/16 19:10:20 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*my_concat(char *str, char c)
{
	int		i;
	char	*new;
	size_t	len;

	i = -1;
	len = my_strlen(str);
	new = malloc(sizeof(*new) * (len + 2));
	if (str)
		new = my_strcpy(str, new);
	new[len] = c;
	new[len + 1] = '\0';
	free(str);
	return (new);
}

static int	reverse_comp(char *s1, char *s2)
{
	ssize_t	len1;
	ssize_t	len2;

	if (!s2)
		return (-1);
	len2 = my_strlen(s2) - 1;
	len1 = my_strlen(s1) - 1;
	if (!s1 || len1 < len2)
		return (1);
	while (len2 >= 0)
	{
		if (s1[len1] != s2[len2])
			return (1);
		len2--;
		len1--;
	}
	return (0);
}

static int	double_infile(t_lst *infile)
{
	int		pip[2];
	char	*str;
	char	*input;
	char	*tmp;

	if (pipe(pip) == -1)
		return (-1);
	str = NULL;
	signal(SIGQUIT, SIG_IGN);
	input = readline(">");
	while (input)
	{
		if (!input[0])
		{
			input = readline(">");
			continue ;
		}
		if (!ft_strcmp(infile->str, input))
			break ;
		tmp = my_concat(input, '\n');
		if (!tmp)
		{
			free(str);
			return (-1);
		}
		str = parse_join(str, tmp);
		if (!str)
			return (-1);
		input = readline(">");
	}
	write(pip[1], str, my_strlen(str));
	free(str);
	receive_signal();
	close(pip[1]);
	return (pip[0]);
}

int	multi_outfiles(t_cmd *cmd, int out)
{
	t_lst	*outfiles;
	int		fd;

	outfiles = cmd->outfiles;
	if (!outfiles)
		return (out);
	while (outfiles->next)
	{
		if (outfiles->flag)
			fd = open(outfiles->str, O_CREAT | O_APPEND, 0666);
		else
			fd = open(outfiles->str, O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
			return (-1);
		close(fd);
		outfiles = outfiles->next;
	}
	if (outfiles->flag)
		fd = open(outfiles->str, O_CREAT | O_APPEND | O_RDWR, 0666);
	else
		fd = open(outfiles->str, O_CREAT | O_TRUNC | O_RDWR, 0666);
	return (fd);
}

int	multi_infiles(t_cmd *cmd, int in)
{
	t_lst	*infiles;
	int		fd;

	infiles = cmd->infiles;
	if (!infiles)
		return (in);
	fd = -1;
	while (infiles)
	{
		if (infiles->flag)
		{
			fd = double_infile(infiles);
			if (fd < 0)
				return (-1);
		}
		else
			fd = open(infiles->str, O_RDONLY);
		if (fd < 0)
			return (exit_nopath(cmd, infiles->str, ": No such file or directory\n", 0));
		else if (infiles->next)
			close(fd);
		infiles = infiles->next;
	}
	return (fd);
}
