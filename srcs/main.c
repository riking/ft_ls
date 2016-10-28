/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/10/27 19:22:19 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <libft.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

static void	onerror(char *ctx)
{
	char	*errstr;

	errstr = strerror(errno);
	if (ctx)
		ft_dprintf(2, "%s: %s: %s", ft_progname(), ctx, errstr);
	else
		ft_dprintf(2, "%s: %s", ft_progname(), errstr);
}

/*
** dir->entries:	t_array<t_dirent>
** line:	t_array<char *>
** ary:		t_array<t_array<char *>>
*/

static int	long_list(t_opts opts, t_dir_content *dir)
{
	size_t		idx;
	t_dirent	*e;
	t_array		ary;
	t_array		line;

	ft_printf("total %lu\n", calc_total(dir));
	idx = 0;
	ary = ft_ary_create(sizeof(t_array));
	NGUARD(GFAIL(1, onerror(0)), ary.ptr);
	while (idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		line = render_dirent(opts, e);
		NGUARD(GFAIL(1, onerror(0)), line.ptr);
		ft_ary_append(&ary, &line);
		idx++;
	}
	ZGUARD(GFAIL(1, onerror(0)), print_table(opts, &ary));
	return (0);
}

int			main(int argc, char **argv)
{
	t_dir_content	*dir;
	t_opts			opts;
	t_sort_info		sortinfo;

	(void)argc;
	(void)argv;
	ft_bzero(&opts, sizeof(opts));
	dir = ft_read_dir(".");
	opts.list_long = true;

	sortinfo.func = &sort_name;
	sortinfo.opts = opts;

	ft_ary_sort(&dir->entries, &sort_main, &sortinfo);

	long_list(opts, dir);
	free_dir(dir);
	return (0);
}
