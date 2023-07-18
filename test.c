#include <string.h>
#include <assert.h>
#include "wrap.h"

int
main()
{
	const char *in;
	size_t len, pos;

	in = "a b c"; len = strlen(in);
	pos = wrap_find_break(in, len, 2); assert(pos == 1);
	pos = wrap_find_break(in, len, 3); assert(pos == 3);
	pos = wrap_find_break(in, len, 4); assert(pos == 3);

	in = "Hello, World!"; len = strlen(in);
	pos = wrap_find_break(in, len, 5); assert(pos == 6);
	pos = wrap_find_break(in, len, 6); assert(pos == 6);
	pos = wrap_find_break(in, len, 7); assert(pos == 6);

	in = "Hello,\nWorld!"; len = strlen(in);
	pos = wrap_find_break(in, len, 5); assert(pos == len);
	pos = wrap_find_break(in, len, 6); assert(pos == len);
	pos = wrap_find_break(in, len, 7); assert(pos == len);

	in = "Name:   Mark Freeman"; len = strlen(in);
	pos = wrap_find_break(in, len, 10); assert(pos == 5);
	pos = wrap_find_break(in, len, 11); assert(pos == 5);
	pos = wrap_find_break(in, len, 12); assert(pos == 12);

	in = "Name:\tMark Freeman"; len = strlen(in);
	pos = wrap_find_break(in, len, 10); assert(pos == 5);
	pos = wrap_find_break(in, len, 11); assert(pos == 5);
	pos = wrap_find_break(in, len, 12); assert(pos == 10);

	pos = wrap_find_resume("x", 2);    assert(pos == 0);
	pos = wrap_find_resume(" x", 2);   assert(pos == 1);
	pos = wrap_find_resume("\tx", 2);  assert(pos == 1);
	pos = wrap_find_resume("\nx", 2);  assert(pos == 1);
	pos = wrap_find_resume(" \nx", 3); assert(pos == 2);

	return 0;
}
