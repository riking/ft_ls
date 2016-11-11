/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   columns.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 19:51:08 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 18:58:48 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>

#include <sys/ioctl.h>
#include <stdlib.h>

#define CLEAN_TAB {ft_ary_foreach(&table, &free_array); ft_ary_destroy(&table);}

/*
** table:    t_array<t_array<char*>>
** row:		 t_array<char*>
** namelist: *t_array<char*> // original
** widths:   t_array<int>
*/

static void		free_array(void *ptr, size_t size)
{
	t_array	*ary;

	if (size != sizeof(t_array))
		exit(3);
	ary = ptr;
	ft_ary_destroy(ary);
}

static t_array	one_row(t_array *namelist, size_t offset, size_t num_rows)
{
	size_t	idx;
	t_array	row;

	row = ft_ary_create(sizeof(char*));
	NGUARD(GFAIL(FT_ARY_NULL, (void)0), row.ptr);
	idx = offset;
	while (idx < namelist->item_count)
	{
		ZGUARD(GFAIL(FT_ARY_NULL, ft_ary_destroy(&row)),
				ft_ary_append(&row, ft_ary_get(namelist, idx)));
		idx += num_rows;
	}
	return (row);
}

static t_array	arrange(t_array *namelist, size_t num_rows)
{
	size_t	idx;
	t_array	table;
	t_array	row;

	table = ft_ary_create(sizeof(t_array));
	NGUARD(GFAIL(FT_ARY_NULL, (void)0), table.ptr);
	idx = 0;
	while (idx < num_rows && idx < namelist->item_count)
	{
		row = one_row(namelist, idx, num_rows);
		NGUARD(GFAIL(FT_ARY_NULL, CLEAN_TAB), row.ptr);
		ZGUARD(GFAIL(FT_ARY_NULL, CLEAN_TAB), ft_ary_append(&table, &row));
		idx++;
	}
	return (table);
}

static bool		check(t_array *namelist, size_t num_cols, int twidth, size_t *rows)
{
	int		*widths;
	ssize_t	len;
	size_t	idx;

	widths = ft_memalloc(sizeof(int) * num_cols);
	NGUARD(GFAIL(true, (void)0), widths);
	idx = 0;
	while (idx < namelist->item_count)
	{
		len = color_strlen(*(char**)ft_ary_get(namelist, idx));
		if (widths[idx % num_cols] < len)
			widths[idx % num_cols] = (int)len;
		idx++;
	}
	*rows = (idx / num_cols) + 1;
	len = 0;
	idx = 0;
	while (idx < num_cols)
		len += widths[idx++] + 2;
	free(widths);
	return (len < twidth);
}

int				print_columns(t_opts opts, t_array *namelist)
{
	t_array			table;
	size_t			num_cols;
	size_t			num_rows;
	struct winsize	w;

	w.ws_col = 80;
	ioctl(1, TIOCGWINSZ, &w);
	num_cols = MAX(1, w.ws_col / 2);
	if (!opts.columns)
		num_cols = 1;
	while (num_cols > 1 && !check(namelist, num_cols, w.ws_col, &num_rows))
		num_cols--;
	if (num_cols == 1)
		num_rows = namelist->item_count;
	table = arrange(namelist, num_rows);
	print_table(opts, &table);
	CLEAN_TAB;
	return (0);
}
