/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 15:50:57 by kyork            ###   ########.fr       */
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
		ft_dprintf(2, "%s: %s: %s\n", PROGNAME, ctx, errstr);
	else
		ft_dprintf(2, "%s: %s\n", PROGNAME, errstr);
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

static void	list_argv_dirs(t_opts opts, t_dir_content *dir, bool had_output)
{
	size_t			idx;
	t_dirent		*e;

	idx = -1;
	while (++idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		if (dir->entries.item_count == 1 && !had_output)
			noheader_list(opts, e->fullpath, e->name);
		else
			header_list(opts, e->fullpath, e->name);
	}
}

static int	ary_strcmp(void *left, void *right, size_t size, void *data)
{
	char *a;
	char *b;

	(void)data;
	if (size != sizeof(char*))
		exit(3);
	a = *(char**)left;
	b = *(char**)right;
	return (ft_strcmp(a, b));
}

static void	empty_string_check(char *argv[])
{
	int ac;

	ac = 1;
	while (argv[ac])
	{
		if (argv[ac][0] == '\0')
		{
			errno = ENOENT;
			ft_perror("fts_open");
			exit(1);
		}
		ac++;
	}
}

static void	traverse_argv(t_opts opts, int argc, char *argv[])
{
	t_dir_content	*files;
	t_dir_content	*dirs;
	t_array			argv_ary;

	empty_string_check(argv);
	argv_ary = ft_ary_viewof(argv + 1, argc - 1, sizeof(char*));
	ft_ary_sort(&argv_ary, &ary_strcmp, 0);
	files = stat_argv(opts, argv, &dirs);
	sort_directory(opts, files);
	sort_directory(opts, dirs);
	opts.no_total = 1;
	if (opts.list_long)
		long_list_dir(opts, files);
	else
		short_list_dir(opts, files);
	opts.no_total = 0;
	list_argv_dirs(opts, dirs, argc > 2);
}

int			main(int argc, char **argv)
{
	t_opts			opts;

	ft_set_progname(argv[0]);
	opts = parse_opts(argv);
	if (opts.bad_opt != '\0')
	{
		ft_dprintf(2, "%s: illegal option -- %c\n", argv[0],
				opts.bad_opt);
		ft_dprintf(2, "%s\n", g_usage);
		return (1);
	}
	argv += opts.opt_count;
	argc -= opts.opt_count;
	if (argc == 1)
		traverse_argv(opts, 2, g_argv_dot);
	else
		traverse_argv(opts, argc, argv);
	return (g_rval);
}
