/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:25:20 by kyork             #+#    #+#             */
/*   Updated: 2016/10/27 14:56:08 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# define _DARWIN_FEATURE_64_BIT_INODE
# include <sys/stat.h>

# include <libft.h>
# include <sys/types.h>
# include <stdbool.h>

# define ARRAYLEN(ary) ((ssize_t)(sizeof(ary) / sizeof(*ary)))

# define GFAIL(val, expr) (void)(expr); return (val);
# define ASGUARD(f, v, ...) ({int _r=ft_asprintf(v, __VA_ARGS__);if (_r<0){f}})
# define ZGUARD(fail, expr) if ((expr) != 0) { fail }
# define NGUARD(fail, expr) if ((expr) == NULL) { fail }

# define IS_TYPE(ent, type) ((e->stat.st_mode & S_IFMT) == (type))

typedef struct		s_dirent {
	struct stat		stat;
	char			*name;
	char			*fullpath;
	t_array			print;
	t_array			printlen;
}					t_dirent;

typedef struct		s_dir_content {
	t_array			entries;
	char			*fullpath;
}					t_dir_content;

t_dir_content		*ft_read_dir(char *path);
void				free_dir(t_dir_content *content);
void				free_dirent(void *ptr, size_t size);
void				free_string(void *ptr, size_t size);

typedef struct		s_opts {
	bool			list_recurse:1;
	bool			list_long:1;
	bool			include_dot:1;
	bool			sort_rev:1;
	bool			no_sort:1;
	bool			use_modtime:1;
	bool			use_inodetime:1;
	bool			use_atime:1;
	bool			use_birthtime:1;
	bool			numeric_uids:1; // -n
	bool			list_full_time:1;
}					t_opts;

typedef struct		s_ftype_info {
	mode_t			mask;
	char			chr;
}					t_ftype_info;

char				*render_mode(t_dirent *e);
char				*render_uid(t_opts opts, t_dirent *e);
char				*render_gid(t_opts opts, t_dirent *e);
char				*render_size(t_dirent *e);
char				*render_time(t_opts opts, t_dirent *e);
char				*render_name(t_opts opts, t_dirent *e);

char				*render_name(t_opts opts, t_dirent *e);
int					render_dirent(t_opts opts, t_dirent *e);

#endif
