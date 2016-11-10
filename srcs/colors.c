/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 18:46:30 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 21:14:24 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

size_t			color_strlen(char *s)
{
	size_t	len;
	char	*str;

	len = 0;
	str = s;
	while (*str)
	{
		if (*str == '\033')
		{
			while (*str && *str != 'm')
				str++;
		}
		else
			len++;
		str++;
	}
	return (len);
}

/*
** #NORMAL 00 # no color code at all
** #FILE 00 # regular file: use no color at all
** RESET 0 # reset to "normal" color
** DIR 01;34 # directory
** LINK 01;36 # symbolic link. (If you set this to 'target' instead of a
**  # numerical value, the color is as for the file pointed to.)
**  MULTIHARDLINK 00 # regular file with more than one link
**  FIFO 40;33 # pipe
**  SOCK 01;35 # socket
**  DOOR 01;35 # door
**  BLK 40;33;01 # block device driver
**  CHR 40;33;01 # character device driver
**  ORPHAN 40;31;01 # symlink to nonexistent file, or non-stat'able file ...
**  MISSING 00 # ... and the files they point to
**  SETUID 37;41 # file that is setuid (u+s)
**  SETGID 30;43 # file that is setgid (g+s)
**  CAPABILITY 30;41 # file with capability
**  STICKY_OTHER_WRITABLE 30;42 # dir that is sticky and other-writable (+t,o+w)
**  OTHER_WRITABLE 34;42 # dir that is other-writable (o+w) and not sticky
**  STICKY 37;44 # dir with the sticky bit set (+t) and not other-writable
**  # This is for files with execute permission:
**  EXEC 01;32
*/

#define CSI "\e["
#define SGR "m"

/*
** macro magic from http://stackoverflow.com/questions/16683146/
** copied code ends at the #endif
*/

#if 1
# define GET_COUNT(_1, _2, _3, COUNT, ...) COUNT
# define VA_SIZE(...) GET_COUNT(__VA_ARGS__, 3, 2, 1)
# define CAT(A, B) A ## B
# define SEL_COLOR(NUM) CAT(COLOR_, NUM)
# define COLOR(...) SEL_COLOR(VA_SIZE(__VA_ARGS__))(__VA_ARGS__)
#endif

#define COLOR_1(code1) CSI code1 SGR
#define COLOR_2(code1, code2) CSI code1 SGR CSI code2 SGR
#define COLOR_3(code1, code2, code3) CSI code1 SGR CSI code2 SGR CSI code3 SGR

#define COL_RESET 		COLOR("0")
#define COL_REGULAR		""
#define COL_DIR 		COLOR("01", "34")
#define COL_LINK 		COLOR("01", "36")
#define COL_FIFO 		COLOR("40", "33")
#define COL_SOCK 		COLOR("01", "35")
#define COL_BLK 		COLOR("40", "33", "01")
#define COL_CHR 		COLOR("40", "33", "01")
#define COL_WHITEOUT 	COLOR("47", "30", "01")
#define COL_ORPHAN 		COLOR("40", "31", "01")
#define COL_MISSING 	COLOR("00")
#define COL_SETUID 		COLOR("37", "41")
#define COL_SETGID 		COLOR("30", "43")
#define COL_CAPABILITY 	COLOR("30", "41")
#define COL_STICKY_OTHER_WRITABLE 	COLOR("30", "42")
#define COL_OTHER_WRITABLE 	COLOR("34", "42")
#define COL_STICKY 		COLOR("37", "44")
#define COL_EXEC 		COLOR("01", "32")

t_filename_color	g_filename_color[] = {
	{S_IFREG, "Makefile", COLOR("01", "33")},
	{S_IFREG, "makefile", COLOR("01", "33")},
	{S_IFREG, "author", COLOR("01", "33")},
};

static const char	*special_color(t_opts opts, t_dirent *e)
{
	(void)opts;
	if (IS_TYPE(e, S_IFLNK))
	{
		if (e->broken_link)
			return (COL_ORPHAN);
		return (COL_LINK);
	}
	if (IS_TYPE(e, S_IFIFO))
		return (COL_FIFO);
	if (IS_TYPE(e, S_IFSOCK))
		return (COL_SOCK);
	if (IS_TYPE(e, S_IFBLK))
		return (COL_BLK);
	if (IS_TYPE(e, S_IFCHR))
		return (COL_CHR);
	if (IS_TYPE(e, S_IFWHT))
		return (COL_WHITEOUT);
	return (COL_ORPHAN);
}

static const char	*regfile_color(t_opts opts, t_dirent *e)
{
	if (IS_TYPE(e, S_IFREG))
	{
		if ((e->stat.st_mode & S_ISUID) != 0)
			return (COL_SETUID);
		if ((e->stat.st_mode & S_ISGID) != 0)
			return (COL_SETGID);
		if ((e->stat.st_mode & 0111) != 0)
			return (COL_EXEC);
		return (COL_REGULAR);
	}
	if (IS_TYPE(e, S_IFDIR))
	{
		if ((e->stat.st_mode & S_ISVTX) && (e->stat.st_mode & S_IWOTH))
			return (COL_STICKY_OTHER_WRITABLE);
		if ((e->stat.st_mode & S_IWOTH) != 0)
			return (COL_OTHER_WRITABLE);
		if ((e->stat.st_mode & S_ISVTX) != 0)
			return (COL_STICKY);
		return (COL_DIR);
	}
	return (special_color(opts, e));
}

const char			*get_color(t_opts opts, t_dirent *e)
{
	size_t	idx;

	if (!opts.colors)
		return ("");
	idx = -1;
	while (++idx < ARRAYLEN(g_filename_color))
		if (0 == ft_strcmp(e->name, g_filename_color[idx].name))
			if (IS_TYPE(e, g_filename_color[idx].type))
				return (g_filename_color[idx].color);
	return (regfile_color(opts, e));
}
