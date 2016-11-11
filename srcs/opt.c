/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 20:25:59 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 19:06:23 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>
#include <unistd.h>

const char			*g_usage = "usage: ft_ls [-FGRSTUacdflnprtu1] [file ...]";

static t_option		g_options[] = {
	{'l', OPT_LIST_LONG | OPT_ARGV_NOFOLLOW},
	{'R', OPT_LIST_RECU},
	{'a', OPT_LIST_ALL},
	{'A', OPT_LIST_HIDN},
	{'n', OPT_NUM_UIDS | OPT_LIST_LONG},
	{'T', OPT_FULL_TIME},
	{'G', OPT_FORCE_COLOR},
	{'d', OPT_NO_DIRS | OPT_ARGV_NOFOLLOW},
	{'f', OPT_SORT_NONE | OPT_LIST_ALL},
	{'r', OPT_SORT_REV},
	{'t', OPT_SORT_TIME},
	{'S', OPT_SORT_SIZE},
};

static t_timeopt	g_time_opts[] = {
	{'c', TIME_C},
	{'u', TIME_A},
	{'U', TIME_B},
};

static bool			apply_special(char c, uint32_t *bits, t_opts *opts)
{
	if (c == '1')
	{
		opts->allow_columns = 0;
		*bits &= ~OPT_LIST_LONG;
		return (true);
	}
	if (c == 'p')
		GFAIL(true, opts->name_suffix = 0x1);
	if (c == 'F')
		GFAIL(true, opts->name_suffix = 0x3);
	return (false);
}

static int			apply_opt(char c, uint32_t *bits, t_opts *opts)
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

static void			bits_fields_2(uint32_t bits, t_opts *opts)
{
	if (bits & OPT_SORT_REV)
		opts->sort_rev = 1;
	if (bits & OPT_SORT_NONE)
		opts->sort_none = 1;
	if (bits & OPT_SORT_TIME)
		opts->sort_time = 1;
	if (bits & OPT_SORT_SIZE)
		opts->sort_size = 1;
}

static void			bits_to_fields(uint32_t bits, t_opts *opts)
{
	if (bits & OPT_LIST_LONG)
		opts->list_long = 1;
	if (bits & OPT_LIST_RECU)
		opts->list_recurse = 1;
	if (bits & OPT_NO_DIRS)
		opts->no_dirs = 1;
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
	int			i;
	uint32_t	flags;
	int			ac;

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
