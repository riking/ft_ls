/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 18:47:29 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 15:49:38 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>

#include <stdlib.h>

void	sort_directory(t_opts opts, t_dir_content *dir)
{
	t_sort_info		sortinfo;

	sortinfo.func = &sort_name;
	if (opts.sort_time)
		sortinfo.func = &sort_time;
	sortinfo.opts = opts;
	ft_ary_sort(&dir->entries, &sort_main, &sortinfo);
}

int		sort_main(void *left, void *right, size_t size, void *data)
{
	t_sort_info	*info;

	if (size != sizeof(t_dirent))
		exit(3);
	info = data;
	if (info->opts.sort_rev)
		return (-info->func(info->opts, left, right));
	else
		return (info->func(info->opts, left, right));
}

int		sort_name(t_opts opts, t_dirent *a, t_dirent *b)
{
	(void)opts;
	return (ft_strcmp(a->name, b->name));
}

int		sort_time(t_opts opts, t_dirent *a, t_dirent *b)
{
	struct timespec	left;
	struct timespec	right;

	left = select_time(opts, a);
	right = select_time(opts, b);
	if (left.tv_sec < right.tv_sec)
		return (1);
	else if (left.tv_sec > right.tv_sec)
		return (-1);
	else if (left.tv_nsec < right.tv_nsec)
		return (1);
	else if (left.tv_nsec > right.tv_nsec)
		return (-1);
	else
		return (sort_name(opts, a, b));
}
