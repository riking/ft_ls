/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 20:25:59 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 16:14:37 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>
#include <unistd.h>

const char			*g_usage = "usage: ft_ls [-FGRSTUacdflnprtu1] [file ...]";

static t_option		g_options[] = {
	{'l', OPT_LIST_LONG | OPT_ARGV_NOFOLLOW},
	{'r', OPT_SORT_REV},
	{'R', OPT_LIST_RECU},
	{'a', OPT_LIST_ALL},
	{'A', OPT_LIST_HIDN},
	{'n', OPT_NUM_UIDS | OPT_LIST_LONG},
	{'T', OPT_FULL_TIME},
	{'G', OPT_FORCE_COLOR},
	{'f', OPT_SORT_NONE | OPT_LIST_ALL},
	{'1', OPT_NO_COLUMNS},
	{'d', OPT_NO_DIRS | OPT_ARGV_NOFOLLOW},
	{'p', OPT_DIR_SUFX},
	{'F', OPT_DIR_SUFX | OPT_NAME_SUFX},
};

static t_timeopt	g_time_opts[] = {
	{'c', TIME_C},
	{'u', TIME_A},
	{'U', TIME_B},
};

#include <ft_printf.h>
static bool			apply_special(char c, short *bits, t_opts *opts)
{
	(void)opts;
	if (c == '1')
	{
		*bits |= OPT_NO_COLUMNS;
		*bits &= ~OPT_LIST_LONG;
		return (true);
	}
	if (c == 't')
		GFAIL(true, opts->sort_func = &sort_time);
	if (c == 'S')
		GFAIL(true, opts->sort_func = &sort_size);
	return (false);
}

static int			apply_opt(char c, short *bits, t_opts *opts)
{
	int	i;

	if (apply_special(c, bits, opts))
		return (0);
	i = 0;
	while (i < ARRAYLEN(g_options))
	{
		if (c == g_options[i].chr)
			GFAIL(0, *bits |= g_options[i].bit);
		i++;
	}
	i = -1;
	while (++i < ARRAYLEN(g_time_opts))
		if (c == g_time_opts[i].chr)
		{
			opts->time_field = g_time_opts[i].opt;
			return (0);
		}
	opts->bad_opt = c;
	return (1);
}

static void			bits_fields_2(short bits, t_opts *opts)
{
	if (bits & OPT_DIR_SUFX)
		opts->dir_suffix = 1;
	if (bits & OPT_NAME_SUFX)
		opts->name_suffix = 1;
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
	if (bits & OPT_NO_DIRS)
		opts->no_dirs = 1;
	bits_fields_2(bits, opts);
}

static t_opts		default_opts(void)
{
	t_opts	ret;

	ft_bzero(&ret, sizeof(ret));
	ret.colors = !!isatty(1);
	ret.allow_columns = !!isatty(1);
	ret.time_field = TIME_DEFAULT;
	return (ret);
}

bool				parse_opts(t_opts *opts, char **argv)
{
	int		i;
	short	flags;
	int		ac;

	*opts = default_opts();
	flags = 0;
	ac = 1;
	while (argv[ac])
		if (argv[ac][0] == '-')
		{
			if (argv[ac][1] == 0)
				break ;
			opts->opt_count++;
			if (argv[ac][1] == '-' && argv[ac][2] == 0)
				break ;
			i = 0;
			while (argv[ac][++i])
				ZGUARD(GFAIL(false, (void)0),
						apply_opt(argv[ac][i], &flags, opts));
			ac++;
		}
		else
			break ;
	bits_to_fields(flags, opts);
	return (false);
}
