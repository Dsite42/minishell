NAME=minishell
OBJS=
HDRS=

CFLAGS=-Wall -Wextra -Werror
LIBS=-l readline

include src/Makefile

.PHONY: all clean fclean re

all: $(NAME)
re: fclean all

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

obj/%.o: src/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find obj -type f -name '*.o' -delete

fclean:
	find obj -type f -name '*.o' -delete
	rm -f $(NAME)