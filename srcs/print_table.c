/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 16:17:24 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 21:48:46 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <libft.h>

bool			g_any_output;

/*
** table:	t_array<t_array<char*>>
** line:	t_array<char *>
** widths:	t_array<int>
**
** ft_ary_get returns an unfreeable *T valid until array is modified
*/

t_array			align_table(t_array *table)
{
	t_array	widths;
	t_array	*line;
	size_t	idx[2];
	int		width;

	widths = ft_ary_create(sizeof(int));
	NGUARD(GFAIL(FT_ARY_NULL, (void)0), widths.ptr);
	idx[0] = 0;
	while (idx[0] < table->item_count)
	{
		line = (t_array*)ft_ary_get(table, idx[0]);
		idx[1] = 0;
		while (idx[1] < line->item_count)
		{
			width = color_strlen(*(char**)ft_ary_get(line, idx[1]));
			if (widths.item_count <= idx[1] ||
					width > *(int*)ft_ary_get(&widths, idx[1]))
				ft_ary_set(&widths, &width, idx[1]);
			idx[1]++;
		}
		idx[0]++;
	}
	return (widths);
}

static void		print_elem(t_opts opts, t_array *widths,
					t_array *line, size_t idx)
{
	char	*str;
	int		cwidth;

	str = *(char**)ft_ary_get(line, idx);
	cwidth = *(int*)ft_ary_get(widths, idx);
	if (idx == line->item_count - 1)
		ft_printf("%s\n", str);
	else if (opts.columns ||
			(opts.list_long && (idx == 2 || idx == 3)))
	{
		ft_printf("%s%*s", str, (int)(cwidth - color_strlen(str) + 2), "");
	}
	else
		ft_printf("%*s ", cwidth, str);
}

int				print_table(t_opts opts, t_array *table)
{
	t_array	widths;
	t_array *line;
	size_t	idx[2];

	widths = align_table(table);
	NGUARD(GFAIL(-1, (void)0), widths.ptr);
	idx[0] = -1;
	while (++idx[0] < table->item_count)
	{
		line = ft_ary_get(table, idx[0]);
		idx[1] = -1;
		while (++idx[1] < line->item_count)
		{
			print_elem(opts, &widths, line, idx[1]);
			g_any_output = true;
		}
	}
	ft_ary_destroy(&widths);
	return (0);
}
