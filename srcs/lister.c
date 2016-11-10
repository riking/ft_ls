/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lister.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 14:30:48 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 16:13:13 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <errno.h>

static void	onerror(t_array ary)
{
	ft_perror(NULL);
	free_string_array_array(ary);
}

/*
** dir->entries		t_array<t_dirent>
** line				t_array<char*>
** ary				t_array<t_array<char*>
*/

int			long_list_dir(t_opts opts, t_dir_content *dir)
{
	size_t		idx;
	t_dirent	*e;
	t_array		ary;
	t_array		line;

	ft_printf("total %lu\n", calc_total(dir));
	ary = ft_ary_create(sizeof(t_array));
	NGUARD(GFAIL(0, ft_perror(NULL)), ary.ptr);
	idx = 0;
	while (idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		line = render_dirent(opts, e);
		NGUARD(GFAIL(0, onerror(ary)), line.ptr);
		ZGUARD(GFAIL(0, onerror(ary)), ft_ary_append(&ary, &line));
		idx++;
	}
	print_table(opts, &ary);
	free_string_array_array(ary);
	if (opts.list_recurse)
		recurse_list(opts, dir);
	return (0);
}

int			short_list_dir(t_opts opts, t_dir_content *dir)
{
	size_t		idx;
	t_dirent	*e;
	t_array		ary;
	t_array		line;

	ary = ft_ary_create(sizeof(t_array));
	NGUARD(GFAIL(0, ft_perror(NULL)), ary.ptr);
	idx = 0;
	while (idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		line = render_dirent(opts, e);
		NGUARD(GFAIL(0, onerror(ary)), line.ptr);
		ZGUARD(GFAIL(0, onerror(ary)), ft_ary_append(&ary, &line));
		idx++;
	}
	print_table(opts, &ary);
	free_string_array_array(ary);
	if (opts.list_recurse)
		recurse_list(opts, dir);
	return (0);
}

static void	header_list(t_opts opts, char *fullpath)
{
	t_dir_content	*d;

	ft_printf("\n%s:\n", fullpath);
	d = ft_read_dir(fullpath, opts.all_type);
	if (!d)
	{
		ft_perror(fullpath);
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

void		recurse_list(t_opts opts, t_dir_content *dir)
{
	size_t			idx;
	t_dirent		*e;

	idx = 0;
	while (idx < dir->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&dir->entries, idx);
		if (IS_TYPE(e, S_IFDIR) && 0 != ft_strcmp(e->name, ".") &&
				0 != ft_strcmp(e->name, ".."))
		{
			header_list(opts, e->fullpath);
		}
		idx++;
	}
}
