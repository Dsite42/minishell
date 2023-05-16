#include "builtin.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>

int	builtin_pwd(int argc, char *argv[], int out_fd, void *context)
{
	int		index;
	char	buffer[MAXPATHLEN + 1];

	(void) context;
	index = 1;
	while (index < argc)
	{
		if (argv[index][0] == '-')
		{
			print_fd(STDERR_FILENO, "minishell: pwd: ");
			print_fd(STDERR_FILENO, argv[index]);
			print_fd(STDERR_FILENO, ": invalid option\n");
			return (1);
		}
		index++;
	}
	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("pwd: getcwd() failed");
		return (1);
	}
	print_fd(out_fd, buffer);
	print_fd(out_fd, "\n");
	return (0);
}