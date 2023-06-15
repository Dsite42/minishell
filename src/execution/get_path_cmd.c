#include "execution.h"
#include <unistd.h>
#include <stdlib.h>

static size_t	begin_delimiter2(t_slice slice)
{
	return (slice.size > 0 && *slice.data == ':');
}
static char	*join_path_cmd(t_slice splitted_path, char *argv)
{
	char	*path_cmd;
	char	*path_cmd_start;

	path_cmd = (char *)malloc((splitted_path.size
				+ 1 + ms_str_length(argv) + 1) * sizeof(char));
	path_cmd_start = path_cmd;
	ms_copy(path_cmd, splitted_path.data, splitted_path.size);
	path_cmd = path_cmd + splitted_path.size;
	*path_cmd = '/';
	path_cmd++;
	ms_copy(path_cmd, argv, ms_str_length(argv));
	path_cmd = path_cmd + ms_str_length(argv);
	*path_cmd = '\0';
	return (path_cmd_start);
}

char	*get_path_cmd(char **argv, t_state *state)
{
	t_var	*path_var;
	t_slice	splitted_path;
	t_slice	rest_path;
	char	*to_join[3];
	char	*path_cmd;

	path_var = vars_get(&(state->root_var), slice0("PATH"));

	rest_path.data = slice0(path_var->value).data;
	rest_path.size = slice0(path_var->value).size;
	while (rest_path.size != 0)
	{
		split_once(slice0(rest_path.data), begin_delimiter2,
			&splitted_path, &rest_path);
		rest_path = advance(rest_path);
		path_cmd = join_path_cmd(splitted_path, *argv);
		if (access(path_cmd, F_OK) == 0)
			return (path_cmd);
	}
	return (NULL);
}
