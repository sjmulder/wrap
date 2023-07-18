#include <stdio.h>
#include <string.h>
#include "wrap.h"

const char buf[] =
"It is right that he too should have his little chronicle, his "
"memories, his reason, and be able to recognize the good in the bad, "
"the bad in the worst, and so grow gently old all down the unchanging "
"days and die one day like any other day, only shorter.\n"
"		-- Samuel Beckett, \"Malone Dies\"";

int
main()
{
	size_t off = 0;
	size_t len = strlen(buf);

	while (off < len) {
		size_t wrap_pos = wrap_find_break(buf+off, len-off, 72);

		if (wrap_pos != len) {
			/* wrap at wrap_pos, print the line */
			fwrite(buf+off, 1, wrap_pos, stdout);
			fputc('\n', stdout);

			/* advance cursor; skip past trailing whitespace */
			off += wrap_pos;
			off += wrap_find_resume(buf+off, len-off);
		} else {
			/* nothing left to wrap */
			fwrite(buf+off, 1, len-off, stdout);

			off = len; /* reached end */
		}
	}

	return 0;
}
