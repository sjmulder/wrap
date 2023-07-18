#include <stddef.h>
#include <ctype.h>
#include "wrap.h"

size_t
wrap_find_break(const char *s, size_t len, size_t cols)
{
	int have_space=0, in_space=0;
	size_t col=0, last_spaces=0, i;

	for (i=0; i<len; i++) {
		if (s[i] == '\n')
			col = 0;
		else if (s[i] == '\t')
			col = (col+8)/8*8;
		else if (isprint(s[i]))
			col++;

		if (s[i] == '\n' || s[i] == '\r' || !isspace(s[i]))
			in_space = 0;
		else if (!in_space) {
			in_space = have_space = 1;
			last_spaces = i;
		}

		if (col > cols && have_space && last_spaces != 0)
			return last_spaces;
	}

	return len;
}

size_t
wrap_find_resume(const char *s, size_t len)
{
	size_t i;

	for (i=0; i<len; i++) {
		if (s[i] == '\n')
			return i+1;
		if (!isspace(s[i]))
			return i;
	}

	return len;
}
