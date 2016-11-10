/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 15:41:18 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 17:17:53 by kyork            ###   ########.fr       */
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
	NGUARD(GFAIL(NULL, (void)0), path_dup);
	result = ft_memalloc(sizeof(t_dir_content));
	NGUARD(GFAIL(NULL, free(path_dup)), result);
	result->entries = ft_ary_create(sizeof(t_dirent));
	if (!result->entries.ptr)
	{
		free(path_dup);
		free(result);
		return (NULL);
	}
	result->fullpath = path_dup;
	return (result);
}

static bool				ft_stat(t_dir_content *r, int len, char *name)
{
	t_dirent	ent;

	ft_asprintf(&ent.name, "%.*s", len, name);
	if (r->fullpath[0])
		ft_asprintf(&ent.fullpath, "%s/%s", r->fullpath, ent.name);
	else
		ft_asprintf(&ent.fullpath, "%s", ent.name);
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

t_dir_content			*ft_read_dir(char *path, t_la_type la)
{
	t_dir_content	*result;
	struct dirent	*dp;
	DIR				*dir;

	dir = opendir(path);
	NGUARD(GFAIL(NULL, (void)0), dir);
	result = new_dir(path);
	NGUARD(GFAIL(NULL, closedir(dir)), result);
	while ((dp = readdir(dir)) != NULL)
	{
		if (dp->d_name[0] == '.' && la == LIST_NORMAL)
			continue ;
		if (!ft_stat(result, dp->d_namlen, dp->d_name))
		{
			free_dir(result);
			closedir(dir);
			return (NULL);
		}
	}
	closedir(dir);
	return (result);
}

t_dir_content			*stat_argv(char *argv[],
		t_dir_content **dirs)
{
	t_dir_content	*result;
	t_dirent		*e;
	int				ac;

	NGUARD(GFAIL(NULL, (void)0), result = new_dir(""));
	NGUARD(GFAIL(NULL, free_dir(result)), *dirs = new_dir(""));
	ac = 1;
	while (argv[ac])
	{
		if (!ft_stat(result, ft_strlen(argv[ac]), argv[ac]))
		{
			free_dir(result);
			return (NULL);
		}
		e = (t_dirent*)ft_ary_get(&result->entries,
				result->entries.item_count - 1);
		if (IS_TYPE(e, S_IFDIR))
		{
			ft_ary_append(&(*dirs)->entries, e);
			ft_ary_remove(&result->entries, result->entries.item_count - 1);
		}
		ac++;
	}
	return (result);
}
