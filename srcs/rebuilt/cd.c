/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 11:40:46 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/13 09:38:39 by tpetit           ###   ########.fr       */
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

static char	*get_str(char **env, char *str)
{
	int	i;

	i = 0;
	while (env[i] && my_scmp(env[i], str))
		i++;
	return (env[i]);
}

static char	**free_str(char *str1, char *str2, char **ret)
{
	free(str1);
	free(str2);
	return (ret);
}

static char	**change_pwd(char **env)
{
	char	*tmp;
	char	*tmp_new;
	char	*tmp_old;
	int		i;
	int		j;

	tmp = malloc(sizeof(*tmp) * PATH_MAX);
	if (!tmp)
		return ((void *)0);
	i = 0;
	while (env[i] && my_scmp(env[i], "OLDPWD="))
		i++;
	j = 0;
	while (env[j] && my_scmp(env[j], "PWD="))
		j++;
	tmp_new = my_strjoin("OLD", env[j]);
	tmp_old = my_strjoin("PWD=", getcwd(tmp, PATH_MAX));
	free(tmp);
	if (!tmp_new || !tmp_old)
		return (free_str(tmp_new, tmp_old, env));
	free_str(env[i], env[j], ((void *)0));
	env[j] = tmp_new;
	env[i] = tmp_old;
	return (env);
}

int	get_cd(t_cmd *cmd, char ***env)
{
	int		ret;
	char	*env_var;

	ret = 0;
	if (cmd->flags[1] && cmd->flags[2])
	{
		write(STDERR_FILENO, "Too much arguments for this command.\n", 37);
		return (ERROR);
	}
	else
	{
		if (!cmd->flags[1])
		{
			if (get_str(*env, "HOME=") == NULL)
			{
				write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
				return (1);
			}
			ret = chdir(get_str(*env, "HOME=") + 5);
		}
		else
			ret = chdir(cmd->flags[1]);
		*env = change_pwd(*env);
	}
	if (ret)
	{
		write(STDERR_FILENO, "cd: no such file or directory: ", 31);
		if (!cmd->flags[1])
			write(STDERR_FILENO, get_str(*env, "HOME=") + 5, my_strlen(get_str(*env, "HOME=")) - 5);
		else
			write(STDERR_FILENO, cmd->flags[1], my_strlen(cmd->flags[1]));
		write(STDERR_FILENO, "\n", 1);
	}
	return (ret);
}
