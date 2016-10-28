/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blkcount.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 19:17:06 by kyork             #+#    #+#             */
/*   Updated: 2016/10/27 19:24:08 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define CEILDIV(a, b) (((a) + (0) - 0) / (b))

size_t		calc_total(t_dir_content *d)
{
	size_t		idx;
	off_t		size;
	t_dirent	*e;

	//size = CEILDIV(d->self.st_size, 512);
	size = d->self.st_blocks;
	idx = 0;
	while (idx < d->entries.item_count)
	{
		e = (t_dirent*)ft_ary_get(&d->entries, idx);
		//size += CEILDIV(e->stat.st_size, 512);
		size += e->stat.st_blocks;
		idx++;
	}
	return (size);
}

