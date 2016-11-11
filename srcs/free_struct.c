/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 16:33:46 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 14:58:57 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <stdlib.h>

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

void	free_string(void *ptr, size_t size)
{
	char	**strp;

	if (size != sizeof(char*))
		exit(3);
	strp = (char**)ptr;
	free(*strp);
}

void	free_string_array(void *ptr, size_t size)
{
	t_array	*ary;

	if (size != sizeof(t_array))
		exit(3);
	ary = (t_array*)ptr;
	ft_ary_foreach(ary, &free_string);
	ft_ary_destroy(ary);
}

void	free_string_array_array(t_array ary)
{
	ft_ary_foreach(&ary, &free_string_array);
	ft_ary_destroy(&ary);
}
