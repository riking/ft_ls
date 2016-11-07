/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 18:47:29 by kyork             #+#    #+#             */
/*   Updated: 2016/11/06 16:45:16 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>

#include <stdlib.h>

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
	time_t	left;
	time_t	right;

	left = select_time(opts, a);
	right = select_time(opts, b);
	if (left < right)
		return (-1);
	else if (left > right)
		return (1);
	else
		return (sort_name(opts, a, b));
}
