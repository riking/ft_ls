/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/11/09 16:22:05 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <libft.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

void		ft_perror(char *ctx)
{
	char	*errstr;

	errstr = strerror(errno);
	if (ctx)
		ft_dprintf(2, "%s: %s: %s\n", ft_progname(), ctx, errstr);
	else
		ft_dprintf(2, "%s: %s\n", ft_progname(), errstr);
}

/*
** dir->entries:	t_array<t_dirent>
** line:	t_array<char *>
** ary:		t_array<t_array<char *>>
*/

int			main(int argc, char **argv)
{
	t_dir_content	*dir;
	t_opts			opts;

	ft_set_progname(argv[0]);
	opts = parse_opts(argv);
	if (opts.bad_opt != '\0')
	{
		ft_dprintf(2, "%s: illegal option -- %c", ft_progname(), opts.bad_opt);
		ft_dprintf(2, "usage: %s [-RTUaclnrtu] [file ...]\n", ft_progname());
		return (1);
	}
	argv += opts.opt_count;
	argc -= opts.opt_count;
	ft_printf("argc=%d, argv[argc] = %p", argc, argv[argc]);
	dir = ft_read_dir(".", opts.all_type);
	if (!dir)
	{
		ft_perror(".");
		return (1);
	}
	sort_directory(opts, dir);
	if (opts.list_long)
		long_list_dir(opts, dir);
	else
		short_list_dir(opts, dir);
	free_dir(dir);
	return (0);
}
