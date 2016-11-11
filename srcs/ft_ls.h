/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyork <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 14:25:20 by kyork             #+#    #+#             */
/*   Updated: 2016/11/10 16:27:42 by kyork            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# define _DARWIN_FEATURE_64_BIT_INODE
# include <sys/stat.h>

# include <libft.h>
# include <sys/types.h>
# include <stdbool.h>
# include <stdint.h>
# include <time.h>

# define ARRAYLEN(ary) ((ssize_t)(sizeof(ary) / sizeof(*ary)))

# define GFAIL(val, expr) {(void)(expr); return (val);}
# define GCONT(expr) {(void)expr; continue ;}
# define ASGUARD(f, v, ...) ({int _r=ft_asprintf(v, __VA_ARGS__);if (_r<0){f}})
# define ZGUARD(fail, expr) if ((expr) != 0) { fail }
# define NGUARD(fail, expr) if ((expr) == NULL) { fail }

# define IS_TYPE(ent, type) (((ent)->stat.st_mode & S_IFMT) == (type))

# if 0 || defined(TRUE_CMDNAME)
#  define PROGNAME ft_progname()
# else
#  define PROGNAME "ls"
# endif

extern bool			g_any_output;
extern const char	*g_usage;

typedef struct		s_dirent {
	struct stat		stat;
	char			*name;
	char			*fullpath;
	bool			broken_link;
}					t_dirent;

/*
** entries: t_array<t_dirent>
*/
typedef struct		s_dir_content {
	t_array			entries;
	char			*fullpath;
}					t_dir_content;

typedef enum		e_la_type {
	LIST_NORMAL,
	LIST_HIDDEN,
	LIST_ALL,
}					t_la_type;

void				free_dir(t_dir_content *content);
void				free_dirent(void *ptr, size_t size);
void				free_string(void *ptr, size_t size);

/*
** ptr: *t_array<char*>
*/
void				free_string_array(void *ptr, size_t size);

/*
** ary: t_array<t_array<char*>>
*/
void				free_string_array_array(t_array ary);

typedef enum		e_wtime {
	TIME_M,
	TIME_A,
	TIME_C,
	TIME_B,
}					t_wtime;

# define TIME_DEFAULT TIME_M

# define OPT_LIST_LONG (1 << 0)
# define OPT_LIST_RECU (1 << 1)
# define OPT_SORT_REV  (1 << 2)
# define OPT_SORT_NONE (1 << 3)
# define OPT_SORT_TIME (1 << 4)
# define OPT_SORT_SIZE (1 << 15)
# define OPT_NUM_UIDS  (1 << 5)
# define OPT_FULL_TIME (1 << 6)
# define OPT_LIST_ALL  (1 << 7)
# define OPT_LIST_HIDN (1 << 8)
# define OPT_FORCE_COLOR (1 << 9)
# define OPT_ARGV_NOFOLLOW	(1 << 10)
# define OPT_NO_DIRS 		(1 << 11)
# define OPT_NO_COLUMNS		(1 << 12)
# define OPT_DIR_SUFX		(1 << 13)
# define OPT_NAME_SUFX		(1 << 14)

typedef struct		s_opts {
	int				opt_count;
	t_wtime			time_field;
	t_la_type		all_type;
	uint8_t			list_recurse:1;
	uint8_t			list_long:1;
	uint8_t			sort_none:1;
	uint8_t			sort_rev:1;
	uint8_t			sort_time:1;
	uint8_t			sort_size:1;
	uint8_t			numeric_uids:1;
	uint8_t			list_full_time:1;
	uint8_t			allow_columns:1;
	uint8_t			colors:1;
	uint8_t			argv_nofollow:1;
	uint8_t			no_dirs:1;
	uint8_t			name_suffix:2;

	uint8_t			columns:1;
	uint8_t			no_total:1;
	char			bad_opt;
}					t_opts;

t_dir_content		*ft_read_dir(char *path, t_la_type list_type);
char				*as_readlink(char *path);
t_dir_content		*stat_argv(t_opts opts, char *argv[], t_dir_content **dirs);

typedef struct		s_option {
	char			chr;
	uint32_t		bit;
}					t_option;

typedef struct		s_timeopt {
	char			chr;
	t_wtime			opt;
}					t_timeopt;

bool				parse_opts(t_opts *opts, char **argv);

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

struct timespec		select_time(t_opts opts, t_dirent *e);

char				*render_mode(t_dirent *e);
char				*render_uid(t_opts opts, t_dirent *e);
char				*render_gid(t_opts opts, t_dirent *e);
char				*render_size(t_dirent *e);
char				*render_time(t_opts opts, t_dirent *e);
char				*render_name(t_opts opts, t_dirent *e);

t_array				render_dirent(t_opts opts, t_dirent *e);

typedef struct		s_filename_color {
	mode_t			type;
	char			*name;
	const char		*color;
}					t_filename_color;

# define STR_COL_RESET "\e[0m"

const char			*get_color(t_opts opts, t_dirent *e);
size_t				color_strlen(char *s);

/*
** table: t_array<t_array<char *>>
*/
int					print_table(t_opts opts, t_array *table);

/*
** table:  t_array<t_array<char*>>
** return: t_array<int>
** return[i] is the widest string of table[...][i]
*/
t_array				align_table(t_array *table);

/*
** namelist: t_array<char*>
*/
int					print_columns(t_opts opts, t_array *namelist);

size_t				calc_total(t_dir_content *d);

void				sort_directory(t_opts opts, t_dir_content *d);
int					sort_main(void *left, void *right, size_t size, void *data);
int					sort_name(t_opts opts, t_dirent *a, t_dirent *b);
int					sort_size(t_opts opts, t_dirent *a, t_dirent *b);
int					sort_time(t_opts opts, t_dirent *a, t_dirent *b);

int					long_list_dir(t_opts opts, t_dir_content *d);
int					short_list_dir(t_opts opts, t_dir_content *d);
void				header_list(t_opts opts, char *fullpath, char *name);
void				recurse_list(t_opts opts, t_dir_content *d);

void				ft_perror(char *context);

#endif
