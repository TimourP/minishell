/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 12:23:08 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/02 15:38:29 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int simple_strcmp(char *s1, char *s2)
{
	int i;
	
	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != 0 && s2[i] != 0)
		i++;
	return (s1[i] - s2[i]);
}

int	exec_builtin(t_cmd *cmd, char **env)
{
	int	ret;

	ret = 1;
	if (!simple_strcmp(cmd->cmd, "env"))
		ret = get_env(env, cmd);
	else if (!simple_strcmp(cmd->cmd, "echo"))
		ret = get_echo(cmd);
	else if (!simple_strcmp(cmd->cmd, "pwd"))
		ret = get_pwd(cmd);
	else if (!simple_strcmp(cmd->cmd, "cd"))
		ret = get_cd(cmd, env);
	else if (!simple_strcmp(cmd->cmd, "export"))
		ret = get_export(cmd, &env);
	else if (!simple_strcmp(cmd->cmd, "unset"))
		ret = get_unset(cmd, &env);
	else if (!simple_strcmp(cmd->cmd, "exit"))
		ret = get_exit(cmd, env);
	return (ret);
}