/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:22:44 by kyork             #+#    #+#             */
/*   Updated: 2016/10/24 17:03:28 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <ft_printf.h>
#include <stdlib.h>

static void	print_name(void *ptr, size_t size)
{
	t_dirent	*ent;

	if (size != sizeof(t_dirent))
		exit(3);
	ent = (t_dirent*)ptr;
	ft_printf("%s\n", ent->fullpath);
}

int			main(int argc, char **argv)
{
	t_dir_content	*dir;

	(void)argc;
	(void)argv;
	dir = ft_read_dir(".");
	ft_ary_foreach(&dir->entries, &print_name);
	free_dir(dir);
	return (0);
}
