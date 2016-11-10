/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 17:50:50 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 20:57:32 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>

#include <unistd.h>
#include <stdlib.h>

static char		*as_readlink(t_dirent *e)
{
	char	*l;
	ssize_t	bufsiz;
	ssize_t	ret;

	bufsiz = 32;
	ret = 999;
	l = 0;
	while (ret >= bufsiz - 2)
	{
		bufsiz *= 2;
		free(l);
		l = malloc(bufsiz);
		if (!l)
			return (NULL);
		ret = readlink(e->fullpath, l, bufsiz - 1);
	}
	if (ret == -1)
	{
		free(l);
		return (NULL);
	}
	else
		l[ret] = 0;
	return (l);
}

char			*render_name(t_opts opts, t_dirent *e)
{
	char		*s;
	char		*l;
	const char	*c;

	c = get_color(opts, e);
	if (opts.list_long && IS_TYPE(e, S_IFLNK))
	{
		l = as_readlink(e);
		ft_asprintf(&s, "%s%s%s -> %s", c, e->name,
				opts.colors ? STR_COL_RESET : "", l);
		free(l);
	}
	else
		ft_asprintf(&s, "%s%s%s", c, e->name, opts.colors ? STR_COL_RESET : "");
	return (s);
}
