NAME		=	test_task

CC 			=	gcc
CFLAGS		=	-Wall -Wextra -Werror

SRC			=	fill_app_data.c \
				init_memory.c \
				main.c \
				parse_tlv.c \
				print_data.c \
				sort.c \
				tag_len_check.c \
				utils.c

OBJS		=	$(SRC:.c=.o)

HEADER		=	sel_ppse.h

%.o: %.c sel_ppse.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY		:	all clean fclean re 

all			:	$(NAME)

$(NAME)		: $(OBJS) $(HEADER)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean		:
	rm -f $(OBJS)

fclean		:	clean
	rm -f $(NAME) 

re			:	fclean all