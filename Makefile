SRCS=env.c main.c slice_core.c slice_str.c vars.c
HDRS=builtin.h env.h slice.h vars.h

.PHONY: clean

minishell: $(SRCS) $(HDRS)
	$(CC) -Wall -Wextra -Ofast -o $@ $(SRCS)

clean:
	rm -f minishell