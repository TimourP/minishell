/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:48:02 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/17 13:05:44 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	my_scmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (s1[i] && s2[i])
		return (s1[i] - s2[i]);
	return (s1[i - 1] - s2[i - 1]);
}

static char	**copy_chunks(char **tab, char *str)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (i < (int)my_strlen(str))
	{
		j = 0;
		while (str[i + j])
			if (str[i + j++] == ':')
				break ;
		if (!str[i + j])
			j++;
		tab[count] = malloc(sizeof(**tab) * j + 1);
		if (!tab[count])
			return (free_tab(tab));
		tab[count] = my_strncpy(str + i, tab[count], j);
		tab[count][j - 1] = '/';
		tab[count][j] = '\0';
		count++;
		i += j;
	}
	return (tab);
}

static char	**my_simple_split(char *str, char c)
{
	int		count;
	int		i;
	char	**tab;

	if (!str)
		return ((void *)0);
	i = 0;
	count = 0;
	while (str[i])
		if (str[i++] == c)
			count++;
	tab = malloc(sizeof(*tab) * (count + 2));
	if (!tab)
		return ((void *)0);
	tab[count + 1] = (void *)0;
	tab = copy_chunks(tab, str);
	return (tab);
}

static void	path_check(int ret, char **paths, char ***env, t_cmd *pip)
{
	int		i;
	char	*full_cmd;

	i = -1;
	full_cmd = ((void *)0);
	while (ret < 0 && paths[++i])
	{
		full_cmd = my_strjoin(paths[i], pip->cmd);
		ret = execve(full_cmd, pip->flags, *env);
	}
	free(full_cmd);
	free_tab(paths);
	if (ret < 0 && !pip->infiles && !pip->outfiles)
		exit_nopath(pip, pip->cmd, ": Command not found.\n", 1);
	if (pip->cmd)
		get_exit(EXIT_SUCCESS, pip);
	else
		get_exit(NO_COMMAND, pip);
}

void	my_command(t_cmd *pip, char *cmd, char **argv, char ***env)
{
	char	**paths;
	int		i;
	int		ret;

	ret = exec_builtin(pip, env);
	if (ret != 257 && !init_edit_shell(0, NULL, ret))
		return ;
	i = 0;
	while ((*env)[i] && my_scmp((*env)[i], "PATH="))
		i++;
	if (!(*env)[i])
	{
		execve(cmd, argv, *env);
		exit_nopath(pip, pip->cmd, ": No such file or directory\n", 0);
	}
	ret = -1;
	if (my_strlen(cmd) >= 2 && cmd[0] == '.' && cmd[1] == '/')
		ret = execve(cmd, argv, *env);
	paths = my_simple_split((*env)[i] + 5, ':');
	if (!paths)
		get_exit(MALLOC_ERROR, pip);
	path_check(ret, paths, env, pip);
}
