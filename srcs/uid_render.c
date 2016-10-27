/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uid_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 15:15:45 by kyork             #+#    #+#             */
/*   Updated: 2016/10/27 15:39:16 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>

#define SIX_MONTHS ((365 / 2) * 86400)

char		*render_uid(t_opts opts, t_dirent *e)
{
	struct passwd	*p;
	char			*s;

	p = NULL;
	if (!opts.numeric_uids)
		p = getpwuid(e->stat.st_uid);
	if (p != NULL)
		ASGUARD(GFAIL(NULL, (void)0), &s, "%s", p->pw_name);
	else
		ASGUARD(GFAIL(NULL, (void)0), &s, "%d", e->stat.st_uid);
	return (s);
}

char		*render_gid(t_opts opts, t_dirent *e)
{
	struct group	*g;
	char			*s;

	g = NULL;
	if (!opts.numeric_uids)
		g = getgrgid(e->stat.st_gid);
	if (g != NULL)
		ASGUARD(GFAIL(NULL, (void)0), &s, "%s", g->gr_name);
	else
		ASGUARD(GFAIL(NULL, (void)0), &s, "%d", e->stat.st_gid);
	return (s);
}

time_t		select_time(t_opts opts, t_dirent *e)
{
	if (opts.time_field == TIME_M)
		return (e->stat.st_mtimespec.tv_sec);
	if (opts.time_field == TIME_A)
		return (e->stat.st_atimespec.tv_sec);
	if (opts.time_field == TIME_C)
		return (e->stat.st_ctimespec.tv_sec);
	if (opts.time_field == TIME_B)
		return (e->stat.st_birthtimespec.tv_sec);
	exit(3);
}

/*
** Thu Nov 24 18:22:48 1986n0
** 01234567890123456789012345
**     ^^^^^^^^^^^^
**     ^^^^^^^        ^^^^^
** ^^^^^^^^^^^^^^^^^^^^^^^^
*/

char		*render_time(t_opts opts, t_dirent *e)
{
	time_t	filetime;
	time_t	now;
	char	*ct;
	char	*s;

	filetime = select_time(opts, e);
	time(&now);
	ct = ctime(&filetime);
	if (opts.list_full_time)
		ASGUARD(GFAIL(NULL, (void)0), &s, "%.20s", ct + 4);
	else if (filetime + SIX_MONTHS > now || filetime < now + SIX_MONTHS)
		ASGUARD(GFAIL(NULL, (void)0), &s, "%.7s%.5s", ct + 4, ct + 11);
	else
		ASGUARD(GFAIL(NULL, (void)0), &s, "%.7s%.5s", ct + 4, ct + 19);
	return (s);
}
