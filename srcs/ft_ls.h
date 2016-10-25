/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:25:20 by kyork             #+#    #+#             */
/*   Updated: 2016/10/24 16:12:16 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# define _DARWIN_FEATURE_64_BIT_INODE
# include <sys/stat.h>

# include <libft.h>
# include <sys/types.h>
# include <stdbool.h>

typedef struct		s_dirent {
	struct stat		stat;
	char			*name;
	char			*fullpath;
}					t_dirent;

typedef struct		s_dir_content {
	t_array			entries;
	char			*fullpath;
}					t_dir_content;

t_dir_content		*ft_read_dir(char *path);
void				free_dir(t_dir_content *content);
void				free_dirent(void *ptr, size_t size);

#endif
