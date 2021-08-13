/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 10:57:19 by ldelmas           #+#    #+#             */
/*   Updated: 2021/08/13 09:26:22 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_exit(int status, t_cmd *cmd)
{
	int	do_print;

	do_print = 0;
	if (!cmd || (cmd == init_edit_shell(2, NULL, 0)->start_cmd && !cmd->next
			&& status != MY_FILE_NOT_FOUND))
		do_print = 1;
	init_edit_shell(3, NULL, 0);
	if (status == MALLOC_ERROR)
		write(STDERR_FILENO, "minishell: malloc error\n", 24);
	if (do_print)
		write(STDERR_FILENO, "exit\n", 5);
	exit(status);
}
