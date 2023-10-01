NAME = ft_nm

CC = gcc
CFLAGS = \
	-fsanitize=address -g
RM = rm -rf

SRCS = $(wildcard src/*.c) $(wildcard src/32/*.c) $(wildcard src/64/*.c)
OBJS = $(SRCS:.c=.o)
LIBS = libft/libft.a

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

all: $(NAME)

$(LIBS):
	make all -C libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make clean -C libft

fclean: clean
	$(RM) $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: clean fclean re all
