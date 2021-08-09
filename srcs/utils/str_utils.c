/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 11:46:52 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/04 11:56:58 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	number_len(int n)
{
	int	i;

	i = 0;
	if (n < 0)
		i = 1;
	while (n && ++i)
		n = n / 10;
	return (i);
}

char	*ft_itoa(int n)
{
	const size_t	n_len = number_len(n);
	char			*num;
	int				i;
	int				neg;

	i = -1;
	neg = 0;
	if (n < 0)
		neg = 1;
	num = malloc(sizeof(char) * (n_len + 1 + (int)(n == 0)));
	if (!num)
		return (NULL);
	num[n_len + (int)(n == 0)] = 0;
	if (neg)
		num[0] = '-';
	if (n == 0)
		num[0] = '0';
	while (n && ++i > -1)
	{
		num[n_len - 1 - i] = '0' + n % 10;
		if (neg)
			num[n_len - 1 - i] = '0' + n % 10 * -1;
		n = n / 10;
	}
	return (num);
}