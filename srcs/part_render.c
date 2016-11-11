/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strmode.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/27 13:59:28 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 21:20:47 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>

#include <sys/xattr.h>
#include <sys/acl.h>

static t_ftype_info	g_ftype_info[] = {
	{0, '?'},
	{S_IFREG, '-'},
	{S_IFBLK, 'b'},
	{S_IFCHR, 'c'},
	{S_IFDIR, 'd'},
	{S_IFLNK, 'l'},
	{S_IFIFO, 'p'},
	{S_IFSOCK, 's'},
	{S_IFWHT, 'w'},
};

static bool			get_acl(t_dirent *e)
{
	acl_t		acl;
	acl_entry_t	dummy;
	bool		had_acl;

	acl = acl_get_link_np(e->fullpath, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &dummy) == -1)
	{
		acl_free(acl);
		return (false);
	}
	had_acl = (acl != NULL);
	acl_free(acl);
	return (had_acl);
}

char				*render_mode(t_dirent *e)
{
	char	buf[12];
	ssize_t	i;

	ft_bzero(buf, 12);
	i = 0;
	buf[0] = g_ftype_info[0].chr;
	while (++i < ARRAYLEN(g_ftype_info))
		if (IS_TYPE(e, g_ftype_info[i].mask))
			buf[0] = g_ftype_info[i].chr;
	i = -1;
	while (++i < 9)
		buf[i + 1] = (e->stat.st_mode & (1 << (8 - i))) ? "rwxrwxrwx"[i] : '-';
	if (e->stat.st_mode & S_ISUID)
		buf[3] = (e->stat.st_mode & (0100)) != 0 ? 's' : 'S';
	if (e->stat.st_mode & S_ISGID)
		buf[6] = (e->stat.st_mode & (0010)) != 0 ? 's' : 'S';
	if (e->stat.st_mode & S_ISVTX)
		buf[9] = (e->stat.st_mode & (0001)) != 0 ? 't' : 'T';
	i = listxattr(e->fullpath, NULL, 0, XATTR_NOFOLLOW);
	if (i > 0)
		buf[10] = '@';
	else
		buf[10] = (get_acl(e)) ? '+' : ' ';
	buf[11] = '\0';
	return (ft_strdup(buf));
}

char				*render_size(t_dirent *e)
{
	int		min;
	int		maj;
	char	*s;

	if (IS_TYPE(e, S_IFCHR) || IS_TYPE(e, S_IFBLK))
	{
		min = minor(e->stat.st_rdev);
		maj = major(e->stat.st_rdev);
		if (min > 255 || min < 0)
			ASGUARD(GFAIL(NULL, (void)0), &s, "%3d, 0x%08x", maj,
					(unsigned int)min);
		else
			ASGUARD(GFAIL(NULL, (void)0), &s, "%3d, %3d", maj, min);
	}
	else
	{
		ASGUARD(GFAIL(NULL, (void)0), &s, "%lld", e->stat.st_size);
	}
	return (s);
}
