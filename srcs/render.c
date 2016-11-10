/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 12:31:40 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 17:38:16 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>

#include <stdlib.h>
#include <unistd.h>

#define ARY_ADD(ary, s) ZGUARD({free(s); return (-1);}, ft_ary_append(ary, &s))

/*
** guard macros:
** ASGUARD(&str, fail_action, fmt, ...) - if ft_asprintf fails
** ZGUARD(fail_action, call) - fail if call returns nonzero
** NGUARD(fail_action, call) - fail if call returns null
**
** GFAIL(retval, expr) - do expr, then return retval
*/

static int			long_list(t_opts opts, t_array *ary, t_dirent *e)
{
	char	*s;

	NGUARD(GFAIL(1, (void)0), s = render_mode(e));
	ARY_ADD(ary, s);
	ASGUARD(GFAIL(1, (void)0), &s, "%d", e->stat.st_nlink);
	ARY_ADD(ary, s);
	NGUARD(GFAIL(1, (void)0), s = render_uid(opts, e));
	ARY_ADD(ary, s);
	NGUARD(GFAIL(1, (void)0), s = render_gid(opts, e));
	ARY_ADD(ary, s);
	NGUARD(GFAIL(1, (void)0), s = render_size(e));
	ARY_ADD(ary, s);
	NGUARD(GFAIL(1, (void)0), s = render_time(opts, e));
	ARY_ADD(ary, s);
	return (0);
}

char				*render_name(t_opts opts, t_dirent *e)
{
	char	*s;
	char	*l;
	ssize_t	bufsiz;
	ssize_t	ret;

	if (opts.list_long && IS_TYPE(e, S_IFLNK))
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
		ft_asprintf(&s, "%s -> %s", e->name, l);
		free(l);
		return (s);
	}
	ASGUARD(GFAIL(NULL, (void)0), &s, "%s", e->name);
	return (s);
}

static void			cleanup(void *freeme, t_array *ary)
{
	free(freeme);
	ft_ary_foreach(ary, &free_string);
	ft_ary_destroy(ary);
}

t_array				render_dirent(t_opts opts, t_dirent *e)
{
	char	*s;
	t_array	ary;

	ary = ft_ary_create(sizeof(char*));
	NGUARD(GFAIL(FT_ARY_NULL, cleanup(0, &ary)), ary.ptr);
	if (opts.list_long)
	{
		if (0 != long_list(opts, &ary, e))
		{
			GFAIL(FT_ARY_NULL, cleanup(0, &ary));
		}
	}
	NGUARD(GFAIL(FT_ARY_NULL, cleanup(0, &ary)), s = render_name(opts, e));
	ZGUARD(GFAIL(FT_ARY_NULL, cleanup(s, &ary)), ft_ary_append(&ary, &s));
	return (ary);
}
