SRCS=main.c util.c vars.c
HDRS=util.h vars.h

.PHONY: clean

mk_shell: $(SRCS) $(HDRS)
	$(CC) -Wall -Wextra -Werror -Ofast -o $@ $(SRCS)

clean:
	rm mk_shell