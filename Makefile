SRCS=main.c slice.c vars.c
HDRS=slice.h vars.h

.PHONY: clean

mk_shell: $(SRCS) $(HDRS)
	$(CC) -Wall -Wextra -Werror -Ofast -o $@ $(SRCS)

clean:
	rm -f mk_shell