/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 20:25:59 by kyork             #+#    #+#             */
/*   Updated: 2016/11/06 16:45:39 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>

static t_option	g_options[] = {
	{'l', OPT_LIST_LONG},
	{'r', OPT_SORT_REV},
	{'R', OPT_LIST_RECU},
	{'a', OPT_LIST_ALL},
	{'A', OPT_LIST_HIDN},
	{'t', OPT_SORT_TIME},
	{'n', OPT_NUM_UIDS},
	{'T', OPT_FULL_TIME},
};

static int	apply_opt(char c, short *bits)
{
	int	i;

	i = 0;
	while (i < ARRAYLEN(g_options))
	{
		if (c == g_options[i].chr)
		{
			*bits |= g_options[i].bit;
			return (0);
		}
		i++;
	}
	return (1);
}

static void	bits_to_fields(short bits, t_opts *opts)
{
	if (bits & OPT_LIST_LONG)
		opts->list_long = 1;
	if (bits & OPT_LIST_RECU)
		opts->list_recurse = 1;
	if (bits & OPT_SORT_REV)
		opts->sort_rev = 1;
	if (bits & OPT_SORT_TIME)
		opts->sort_time = 1;
	if (bits & OPT_NUM_UIDS)
		opts->numeric_uids = 1;
	if (bits & OPT_FULL_TIME)
		opts->list_full_time = 1;
	if (bits & OPT_LIST_HIDN)
		opts->all_type = LIST_HIDDEN;
	if (bits & OPT_LIST_ALL)
		opts->all_type = LIST_ALL;
}

t_opts	parse_opts(char **argv)
{
	t_opts	ret;
	int		i;
	short	flags;

	ft_bzero(&ret, sizeof(ret));
	ret.time_field = TIME_DEFAULT;
	while (*argv)
		if (**argv == '-')
		{
			i = 0;
			while ((*argv)[++i])
			{
				if (0 != apply_opt((*argv)[i], &flags))
				{
					ret.bad_opt = (*argv)[i];
					return (ret);
				}
			}
			argv++;
		}
	bits_to_fields(flags, &ret);
	return (ret);
}
