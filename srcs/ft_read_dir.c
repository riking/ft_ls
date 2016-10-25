/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 15:41:18 by kyork             #+#    #+#             */
/*   Updated: 2016/10/24 17:01:10 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <libft.h>
#include <ft_printf.h>
#include <stdlib.h>
#include <dirent.h>

static t_dir_content	*new_dir(char *path)
{
	t_dir_content	*result;
	char			*path_dup;

	path_dup = ft_strdup(path);
	if (!path_dup)
		return (NULL);
	result = ft_memalloc(sizeof(t_dir_content));
	if (!result)
		return (NULL);
	result->entries = ft_ary_create(sizeof(t_dirent));
	if (!result->entries.ptr)
	{
		free(result);
		return (NULL);
	}
	result->fullpath = path_dup;
	return (result);
}

static bool		ft_stat(t_dir_content *r, int len, char *name)
{
	t_dirent	ent;

	ft_asprintf(&ent.name, "%.*s", len, name);
	ft_asprintf(&ent.fullpath, "%s/%s", r->fullpath, ent.name);
	if (!ent.name || !ent.fullpath)
	{
		free_dirent(&ent, sizeof(t_dirent));
		return (false);
	}
	if (0 != lstat(ent.fullpath, &ent.stat))
	{
		free_dirent(&ent, sizeof(t_dirent));
		return (false);
	}
	if (0 != ft_ary_append(&(r->entries), &ent))
	{
		free_dirent(&ent, sizeof(t_dirent));
		return (false);
	}
	return (true);
}

t_dir_content	*ft_read_dir(char *path)
{
	t_dir_content	*result;
	struct dirent	*dp;
	DIR				*dir;

	dir = opendir(path);
	if (!dir)
		return (NULL);
	result = new_dir(path);
	if (!result)
		return (NULL);
	while ((dp = readdir(dir)) != NULL)
	{
		if (!ft_stat(result, dp->d_namlen, dp->d_name))
		{
			free_dir(result);
			return (NULL);
		}
	}
	closedir(dir);
	return (result);
}
