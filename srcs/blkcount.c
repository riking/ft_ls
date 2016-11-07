/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blkcount.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 19:17:06 by kyork             #+#    #+#             */
/*   Updated: 2016/11/06 16:09:05 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define CEILDIV(a, b) (((a) + (0) - 0) / (b))

size_t		calc_total(t_dir_content *d)
{
	size_t		idx;
	off_t		size;
	off_t		blks;
	t_dirent	*e;

	size = 0;
	idx = 0;
	while (idx < d->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&d->entries, idx);
		blks = e->stat.st_blocks;
		size += blks;
		idx++;
	}
	return (size);
}

