 /* libc */
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
 /* unix */
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <sysexits.h>
#include <err.h>
 /* user */
#include "wrap.h"

static noreturn void
usage()
{
	fputs("usage: wrap [-n width] [filename]", stderr);
	exit(EX_USAGE);
}

int
main(int argc, char **argv)
{
	static char buf[4096];
	const char *in_name="<stdin>";
	int cols=72, in_fd=STDIN_FILENO, eof=0, c;
	size_t len=0, wrap_pos, resume_pos;
	ssize_t n_read;

	while ((c = getopt(argc, argv, "hn:")) != -1)
		switch (c) {
		case 'n':
			if ((cols = atoi(optarg)) < 1)
				errx(EX_USAGE, "bad -n");
			break;
		default:
			usage();
		}

	argc -= optind;
	argv += optind;

	if (argc > 1)
		errx(EX_USAGE, "too many arguments (see -h)");
	else if (argc == 0 && isatty(STDIN_FILENO))
		errx(EX_USAGE, "stdin is a TTY, specify file or '-'");
	else if (argc == 1 && strcmp(argv[0], "-") != 0) {
		in_name = argv[0];
		if ((in_fd = open(in_name, O_RDONLY)) == -1)
			err(1, "%s", in_name);
	}

	while (len || !eof) {
		if (!eof && len < sizeof(buf)) {
			n_read = read(in_fd, buf, sizeof(buf) - len);
			if (n_read == -1)
				err(1, "%s", in_name);
			eof = !n_read;
			len += (size_t)n_read;
		}

		wrap_pos = wrap_find_break(buf, len, cols);

		if (wrap_pos < len) {
			fwrite(buf, 1, wrap_pos, stdout);
			fputc('\n', stdout);

			resume_pos = wrap_pos + wrap_find_resume(
			    buf+wrap_pos, len-wrap_pos);
			memmove(buf, buf+resume_pos, len-resume_pos);
			len -= resume_pos;
		} else {
			fwrite(buf, 1, len, stdout);
			len = 0;
		}
	}

	return 0;
}
