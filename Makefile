SRCS=env.c main.c slice_core.c slice_str.c vars.c
HDRS=env.h slice.h vars.h

.PHONY: clean

mk_shell: $(SRCS) $(HDRS)
	$(CC) -Wall -Wextra -Ofast -o $@ $(SRCS)

clean:
	rm -f mk_shell