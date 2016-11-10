# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyork <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/09 19:11:26 by kyork             #+#    #+#              #
#    Updated: 2016/11/09 20:35:54 by kyork            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ft_ls

FILENAMES	+= ft_read_dir.c main.c free_struct.c
FILENAMES	+= render.c colors.c sorting.c
FILENAMES	+= part_render.c uid_render.c name_render.c
FILENAMES	+= print_table.c blkcount.c columns.c
FILENAMES	+= opt.c lister.c

OBJS		= $(addprefix build/, $(FILENAMES:.c=.o))
LIBS		= libft/libft.a libft/libftprintf.a

CFLAGS		+= -Wall -Wextra -Wmissing-prototypes
#CFLAGS		= -Wall -Wextra -Wfloat-equal -Wundef -Wint-to-pointer-cast -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wcast-qual -Wmissing-prototypes -Wstrict-overflow=5 -Wwrite-strings -Wconversion --pedantic-errors
CFLAGS		+= -I includes/
LDFLAGS		+= -Wall -Wextra

ifndef NO_WERROR
	CFLAGS += -Werror
	LDFLAGS += -Werror
endif

ifdef DBUG
	CFLAGS += -fsanitize=address -g
	LDFLAGS += -fsanitize=address -g
endif

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -L libft -lft -lftprintf

install: $(NAME)
	cp $(NAME) $(HOME)/bin/$(NAME)

libft/libft.a:
	make -C libft libft.a

libft/libftprintf.a:
	make -C libft libftprintf.a

clean:
	rm -rf build
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

build:
	mkdir -p build

build/%.o: srcs/%.c srcs/*.h | build
	$(CC) $(CFLAGS) -c -o $@ $<

smalltest: tests/small_test.c $(NAME) | build
	$(CC) $(CFLAGS) -c $< -o build/small_test.o
	$(CC) $(LDFLAGS) -o $@ build/small_test.o -L. -lftprintf

build/fuzz_main.o: tests/fuzz_main.c srcs/*.h | build
	$(CC) $(CFLAGS) -c $< -o build/small_test.o -I ../libft/includes -I $(DYNCALL_PATH)/include

fuzzmain: build/fuzz_main.o $(NAME) | build
	$(CC) $(LDFLAGS) -o $@ build/small_test.o -L. -lftprintf -L ../libft -lft -L $(DYNCALL_PATH)/lib -ldyncall_s
