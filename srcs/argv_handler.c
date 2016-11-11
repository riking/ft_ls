/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/10 21:45:50 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 21:45:52 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#include <stdlib.h>
#include <errno.h>

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

	idx = 0;
	while (idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		if (dir->entries.item_count == 1 && !had_output)
			noheader_list(opts, e->fullpath, e->name);
		else
			header_list(opts, e->fullpath, e->name);
		idx++;
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

void		traverse_argv(t_opts opts, int argc, char *argv[])
{
	t_dir_content	*files;
	t_dir_content	*dirs;
	t_array			argv_ary;

	empty_string_check(argv);
	argv_ary = ft_ary_viewof(argv + 1, argc - 1, sizeof(char*));
	ft_ary_sort(&argv_ary, &ary_strcmp, 0);
	files = stat_argv(opts, argv, &dirs);
	if (!files || !dirs)
		return ;
	sort_directory(opts, files);
	sort_directory(opts, dirs);
	opts.no_total = 1;
	if (opts.list_long)
		long_list_dir(opts, files);
	else
		short_list_dir(opts, files);
	opts.no_total = 0;
	list_argv_dirs(opts, dirs, argc > 2);
	free_dir(files);
	free_dir(dirs);
}
