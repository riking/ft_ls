/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 20:25:59 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 14:20:49 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>
#include <unistd.h>

const char			*g_usage = "usage: ft_ls [-GRTUacdflnrtu1] [file ...]";

static t_option		g_options[] = {
	{'l', OPT_LIST_LONG | OPT_ARGV_NOFOLLOW},
	{'r', OPT_SORT_REV},
	{'R', OPT_LIST_RECU},
	{'a', OPT_LIST_ALL},
	{'A', OPT_LIST_HIDN},
	{'t', OPT_SORT_TIME},
	{'n', OPT_NUM_UIDS | OPT_LIST_LONG},
	{'T', OPT_FULL_TIME},
	{'G', OPT_FORCE_COLOR},
	{'f', OPT_SORT_NONE | OPT_LIST_ALL},
	{'1', OPT_NO_COLUMNS},
	{'d', OPT_NO_DIRS | OPT_ARGV_NOFOLLOW},
};

static t_timeopt	g_time_opts[] = {
	{'c', TIME_C},
	{'u', TIME_A},
	{'U', TIME_B},
};

static int			apply_opt(char c, short *bits, t_opts *opts)
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
	i = -1;
	while (++i < ARRAYLEN(g_time_opts))
		if (c == g_time_opts[i].chr)
		{
			opts->time_field = g_time_opts[i].opt;
			return (0);
		}
	return (1);
}

static void			bits_to_fields(short bits, t_opts *opts)
{
	if (bits & OPT_LIST_LONG)
		opts->list_long = 1;
	if (bits & OPT_LIST_RECU)
		opts->list_recurse = 1;
	if (bits & OPT_SORT_REV)
		opts->sort_rev = 1;
	if (bits & OPT_SORT_NONE)
		opts->sort_none = 1;
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
	if (bits & OPT_FORCE_COLOR)
		opts->colors = 1;
	if (bits & OPT_ARGV_NOFOLLOW)
		opts->argv_nofollow = 1;
	if (bits & OPT_NO_COLUMNS)
		opts->allow_columns = 0;
	if (bits & OPT_NO_DIRS)
		opts->no_dirs = 1;
}

t_opts				parse_opts(char **argv)
{
	t_opts	ret;
	int		i;
	short	flags;

	ft_bzero(&ret, sizeof(ret));
	ret.colors = !!isatty(1);
	ret.allow_columns = !!isatty(1);
	ret.time_field = TIME_DEFAULT;
	flags = 0;
	while (*++argv)
		if (**argv == '-')
		{
			ret.opt_count++;
			if (argv[0][1] == '-' && argv[0][2] == 0)
				break ;
			i = 0;
			while ((*argv)[++i])
			{
				if (0 != apply_opt((*argv)[i], &flags, &ret))
				{
					ret.bad_opt = (*argv)[i];
					return (ret);
				}
			}
		}
	bits_to_fields(flags, &ret);
	return (ret);
}
