/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:25:20 by kyork             #+#    #+#             */
/*   Updated: 2016/11/06 16:44:09 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# define _DARWIN_FEATURE_64_BIT_INODE
# include <sys/stat.h>

# include <libft.h>
# include <sys/types.h>
# include <stdbool.h>
# include <time.h>

# define ARRAYLEN(ary) ((ssize_t)(sizeof(ary) / sizeof(*ary)))

# define GFAIL(val, expr) (void)(expr); ft_dprintf(2, "error on %s:%d\n", __FILE__, __LINE__); return (val);
# define ASGUARD(f, v, ...) ({int _r=ft_asprintf(v, __VA_ARGS__);if (_r<0){f}})
# define ZGUARD(fail, expr) if ((expr) != 0) { fail }
# define NGUARD(fail, expr) if ((expr) == NULL) { fail }

# define IS_TYPE(ent, type) ((e->stat.st_mode & S_IFMT) == (type))

typedef struct		s_dirent {
	struct stat		stat;
	char			*name;
	char			*fullpath;
}					t_dirent;

/*
** entries: t_array<t_dirent>
*/
typedef struct		s_dir_content {
	struct stat		self;
	t_array			entries;
	char			*fullpath;
}					t_dir_content;

typedef enum		e_la_type {
	LIST_NORMAL,
	LIST_HIDDEN,
	LIST_ALL,
}					t_la_type;

t_dir_content		*ft_read_dir(char *path, t_la_type list_type);
void				free_dir(t_dir_content *content);
void				free_dirent(void *ptr, size_t size);
void				free_string(void *ptr, size_t size);

typedef enum		e_wtime {
	TIME_M,
	TIME_A,
	TIME_C,
	TIME_B,
}					t_wtime;

#define TIME_DEFAULT TIME_M

#define OPT_LIST_LONG (1 << 0)
#define OPT_LIST_RECU (1 << 1)
#define OPT_SORT_REV  (1 << 2)
#define OPT_SORT_TIME (1 << 4)
#define OPT_NUM_UIDS  (1 << 5)
#define OPT_FULL_TIME (1 << 6)
#define OPT_LIST_ALL  (1 << 7)
#define OPT_LIST_HIDN (1 << 8)

typedef struct		s_opts {
	char			bad_opt;
	t_wtime			time_field;
	t_la_type		all_type;
	bool			list_recurse:1;
	bool			list_long:1;
	bool			sort_rev:1;
	bool			sort_time:1;
	bool			numeric_uids:1;
	bool			list_full_time:1;

}					t_opts;

typedef struct 		s_option {
	char			chr;
	short			bit;
}					t_option;

t_opts				parse_opts(char **argv);

typedef struct		s_ftype_info {
	mode_t			mask;
	char			chr;
}					t_ftype_info;

typedef int			(*t_dirent_sorter)(t_opts opts,
						t_dirent *left, t_dirent *right);

typedef struct		s_sort_info {
	t_dirent_sorter	func;
	t_opts			opts;
}					t_sort_info;

time_t				select_time(t_opts opts, t_dirent *e);

char				*render_mode(t_dirent *e);
char				*render_uid(t_opts opts, t_dirent *e);
char				*render_gid(t_opts opts, t_dirent *e);
char				*render_size(t_dirent *e);
char				*render_time(t_opts opts, t_dirent *e);
char				*render_name(t_opts opts, t_dirent *e);

char				*render_name(t_opts opts, t_dirent *e);
t_array				render_dirent(t_opts opts, t_dirent *e);

/*
** table: t_array<t_array<char *>>
*/

int					print_table(t_opts opts, t_array *table);

size_t				calc_total(t_dir_content *d);

int					sort_main(void *left, void *right, size_t size, void *data);
int					sort_name(t_opts opts, t_dirent *a, t_dirent *b);
int					sort_time(t_opts opts, t_dirent *a, t_dirent *b);

#endif
