/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 12:31:40 by kyork             #+#    #+#             */
/*   Updated: 2016/10/27 14:56:24 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>

#include <stdlib.h>
#include <unistd.h>

#define ARY_ADD(ary, s) ZGUARD({cleanup(s,e);return (-1);},ft_ary_append(ary,s))

static void			cleanup(char *s, t_dirent *e)
{
	free(s);
	ft_ary_foreach(&e->print, &free_string);
}

/*
** guard macros:
** ASGUARD(&str, fail_action, fmt, ...) - if ft_asprintf fails
** ZGUARD(fail_action, call) - fail if call returns nonzero
** NGUARD(fail_action, call) - fail if call returns null
**
** GFAIL(retval, expr) - do expr, then return retval
*/

static int			long_part1(t_opts opts, t_dirent *e)
{
	char	*s;

	NGUARD(GFAIL(1, cleanup(0, e)), s = render_mode(e));
	ARY_ADD(&e->print, s);
	ASGUARD(GFAIL(1, cleanup(0, e)), &s, "%d", e->stat.st_nlink);
	ARY_ADD(&e->print, s);
	NGUARD(GFAIL(1, cleanup(0, e)), s = render_uid(opts, e));
	ARY_ADD(&e->print, s);
	NGUARD(GFAIL(1, cleanup(0, e)), s = render_gid(opts, e));
	ARY_ADD(&e->print, s);
	NGUARD(GFAIL(1, cleanup(0, e)), s = render_size(e));
	ARY_ADD(&e->print, s);
	NGUARD(GFAIL(1, cleanup(0, e)), s = render_time(opts, e));
	ARY_ADD(&e->print, s);
	return (0);
}

char				*render_name(t_opts opts, t_dirent *e)
{
	char	*s;
	char	*l;
	ssize_t	bufsiz;
	ssize_t	ret;

	(void)opts;
	if (IS_TYPE(e, S_IFLNK))
	{
		bufsiz = 32;
		ret = 999;
		l = 0;
		while (ret >= bufsiz - 2)
		{
			bufsiz *= 2;
			free(l);
			l = malloc(bufsiz);
			ret = readlink(e->fullpath, l, bufsiz - 1);
		}
		l[ret] = 0;
		ASGUARD(GFAIL(NULL, free(l)), &s, "%s -> %s", e->name, l);
		return (s);
	}
	ASGUARD(GFAIL(NULL, (void)0), &s, "%s", e->name);
	return (s);
}

int					render_dirent(t_opts opts, t_dirent *e)
{
	char	*s;

	e->print = ft_ary_create(sizeof(char *));
	if (!e->print.ptr)
		return (1);
	if (opts.list_long)
	{
		ZGUARD(GFAIL(1, cleanup(0, e)), long_part1(opts, e));
	}
	NGUARD(GFAIL(1, cleanup(0, e)), s = render_name(opts, e));
	ARY_ADD(&e->print, s);
	return (0);
}
