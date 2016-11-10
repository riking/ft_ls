/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 13:58:16 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <libft.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

static char	*g_argv_dot[] = {"", ".", NULL};
static int	g_rval;

void		ft_perror(char *ctx)
{
	char	*errstr;

	errstr = strerror(errno);
	if (ctx)
		ft_dprintf(2, "%s: %s: %s\n", ft_progname(), ctx, errstr);
	else
		ft_dprintf(2, "%s: %s\n", ft_progname(), errstr);
	g_rval = 1;
}

static void	noheader_list(t_opts opts, char *fullpath, char *name)
{
	t_dir_content *d;

	d = ft_read_dir(fullpath, opts.all_type);
	if (!d)
	{
		ft_perror(name);
		return ;
	}
	if (d->entries.item_count != 0)
	{
		sort_directory(opts, d);
		if (opts.list_long)
			long_list_dir(opts, d);
		else
			short_list_dir(opts, d);
	}
	free_dir(d);
}

static void	list_argv_dirs(t_opts opts, t_dir_content *dir, bool is_solo)
{
	size_t			idx;
	t_dirent		*e;

	idx = -1;
	while (++idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		if (dir->entries.item_count == 1 && is_solo)
			noheader_list(opts, e->fullpath, e->name);
		else
			header_list(opts, e->fullpath, e->name);
	}
}

static void	traverse_argv(t_opts opts, char *argv[])
{
	t_dir_content	*files;
	t_dir_content	*dirs;

	files = stat_argv(opts, argv, &dirs);
	sort_directory(opts, files);
	sort_directory(opts, dirs);
	opts.no_total = 1;
	if (opts.list_long)
		long_list_dir(opts, files);
	else
		short_list_dir(opts, files);
	opts.no_total = 0;
	list_argv_dirs(opts, dirs, files->entries.item_count == 0);
}

int			main(int argc, char **argv)
{
	t_opts			opts;

	ft_set_progname(argv[0]);
	opts = parse_opts(argv);
	if (opts.bad_opt != '\0')
	{
		ft_dprintf(2, "%s: illegal option -- %c\n", ft_progname(),
				opts.bad_opt);
		ft_dprintf(2, "usage: %s [-RTUaclnrtu] [file ...]\n", ft_progname());
		return (1);
	}
	argv += opts.opt_count;
	argc -= opts.opt_count;
	if (argc == 1)
		traverse_argv(opts, g_argv_dot);
	else
		traverse_argv(opts, argv);
	return (g_rval);
}
