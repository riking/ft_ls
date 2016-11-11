/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 16:51:59 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <libft.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

static char		*g_argv_dot[] = {"", ".", NULL};
static int		g_rval;

void			ft_perror(char *ctx)
{
	char	*errstr;

	errstr = strerror(errno);
	if (ctx)
		ft_dprintf(2, "%s: %s: %s\n", PROGNAME, ctx, errstr);
	else
		ft_dprintf(2, "%s: %s\n", PROGNAME, errstr);
	g_rval = 1;
}

static t_opts	default_opts(void)
{
	t_opts	ret;

	ft_bzero(&ret, sizeof(ret));
	ret.colors = (uint8_t)(isatty(1) != 0);
	ret.allow_columns = (uint8_t)(isatty(1) != 0);
	ret.time_field = TIME_DEFAULT;
	return (ret);
}

int				main(int argc, char **argv)
{
	t_opts			opts;

	ft_set_progname(argv[0]);
	opts = default_opts();
	parse_opts(&opts, argv);
	if (opts.bad_opt != '\0')
	{
		ft_dprintf(2, "%s: illegal option -- %c\n", argv[0],
				opts.bad_opt);
		ft_dprintf(2, "%s\n", g_usage);
		return (1);
	}
	argv += opts.opt_count;
	argc -= opts.opt_count;
	if (argc == 1)
		traverse_argv(opts, 2, g_argv_dot);
	else
		traverse_argv(opts, argc, argv);
	return (g_rval);
}
