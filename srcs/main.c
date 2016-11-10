/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 17:18:51 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <libft.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

void		ft_perror(char *ctx)
{
	char	*errstr;

	errstr = strerror(errno);
	if (ctx)
		ft_dprintf(2, "%s: %s: %s\n", ft_progname(), ctx, errstr);
	else
		ft_dprintf(2, "%s: %s\n", ft_progname(), errstr);
}

static int	ls_one(t_opts opts, char *fname)
{
	t_dir_content	*dir;

	dir = ft_read_dir(fname, opts.all_type);
	if (!dir)
	{
		ft_perror(fname);
		return (1);
	}
	sort_directory(opts, dir);
	if (opts.list_long)
		long_list_dir(opts, dir);
	else
		short_list_dir(opts, dir);
	free_dir(dir);
	return (0);
}

static void	list_argv_dirs(t_opts opts, t_dir_content *dir)
{
	size_t			idx;
	t_dirent		*e;

	idx = -1;
	while (++idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		if (IS_TYPE(e, S_IFDIR))
			header_list(opts, e->fullpath);
	}
}

static int	ls_mult(t_opts opts, char **argv)
{
	t_dir_content	*files;
	t_dir_content	*dirs;

	files = stat_argv(argv, &dirs);
	if (!dirs || !files)
	{
		ft_perror("ls_mult");
		return (1);
	}
	sort_directory(opts, files);
	sort_directory(opts, dirs);
	opts.skip_dirs = 1;
	if (opts.list_long)
		long_list_dir(opts, files);
	else
		short_list_dir(opts, files);
	opts.skip_dirs = 0;
	list_argv_dirs(opts, dirs);
	return (0);
}

int			main(int argc, char **argv)
{
	t_opts			opts;

	ft_set_progname(argv[0]);
	opts = parse_opts(argv);
	if (opts.bad_opt != '\0')
	{
		ft_dprintf(2, "%s: illegal option -- %c\n", ft_progname(), opts.bad_opt);
		ft_dprintf(2, "usage: %s [-RTUaclnrtu] [file ...]\n", ft_progname());
		return (1);
	}
	argv += opts.opt_count;
	argc -= opts.opt_count;
	if (argc == 1)
		return (ls_one(opts, "."));
	else if (argc == 2)
		return (ls_one(opts, argv[1]));
	else
	{
		return (ls_mult(opts, argv));
	}
}
