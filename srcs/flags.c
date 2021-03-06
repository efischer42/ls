/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efischer <efischer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 15:29:10 by efischer          #+#    #+#             */
/*   Updated: 2019/06/08 14:58:54 by efischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "flags.h"

static uint16_t	ft_check_flag(char c, uint16_t flags)
{
	if (c == 'l' && (flags & FLAG_1) == FALSE)
		return (flags |= FLAG_L);
	else if (c == 'a')
		return (flags |= FLAG_A);
	else if (c == 'r')
		return (flags |= FLAG_REV);
	else if (c == 'R')
		return (flags |= FLAG_R);
	else if (c == 't')
		return (flags |= FLAG_T);
	else if (c == 'G')
		return (flags |= FLAG_G);
	else if (c == 'd')
		return (flags |= FLAG_D);
	else if (c == '1')
	{
		flags &= ~FLAG_L;
		return (flags |= FLAG_1);
	}
	else if (c == 'S')
		return (flags |= FLAG_S);
	else if (c == 'n')
		return (flags |= FLAG_N);
	else
		return (FALSE);
}

static void		ft_get_flags(char *arg, uint16_t *flags)
{
	while (*arg != '\0')
	{
		*flags = ft_check_flag(*arg, *flags);
		if (*flags == FALSE)
		{
			ft_printf("ft_ls: illegal option -- %c\n", *arg);
			ft_putendl("usage: ft_ls [-GRadlnrt1] [file ...]");
			exit(EXIT_FAILURE);
		}
		arg++;
	}
}

uint16_t		ft_manage_args(int *current_arg, int ac, char **av)
{
	uint16_t	flags;

	flags = 0;
	while (*current_arg < ac && av[*current_arg][0] == '-'
			&& av[*current_arg][1] != '\0')
	{
		if (ft_strequ(av[*current_arg], "--") == TRUE)
		{
			(*current_arg)++;
			return (flags);
		}
		ft_get_flags(av[*current_arg] + 1, &flags);
		(*current_arg)++;
	}
	return (flags);
}
