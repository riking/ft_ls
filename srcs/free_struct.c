/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 16:33:46 by kyork             #+#    #+#             */
/*   Updated: 2016/10/24 17:01:24 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <stdlib.h>
#include <ft_printf.h>

void	free_dirent(void *ptr, size_t size)
{
	t_dirent	*entry;

	if (size != sizeof(t_dirent))
		exit(3);
	entry = (t_dirent*)ptr;
	free(entry->name);
	free(entry->fullpath);
}

void	free_dir(t_dir_content *content)
{
	ft_ary_foreach(&(content->entries), &free_dirent);
	ft_ary_destroy(&content->entries);
	free(content->fullpath);
	free(content);
}
