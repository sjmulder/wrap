wrap
====
Simple text wrapping CLI and static library. Just for play since POSIX
[fmt](https://man.openbsd.org/fmt) does it better.

Comes with a [sample](sample.c) and man pages ([wrap.1](wrap.1),
[libwrap.3](libwrap.3)).

CLI
---
**wrap** [**-n** *cols*] [*path*]

Wraps the input file *path* (or standard input) to *cols* columns, or
72 by default.

Existing line breaks and leading whitespace are preserved.

Exits 0 on success, `EX_USAGE` (64) on usage error, or 1 on any other
error.

Library
-------
    #include <sample.h>

    size_t wrap_find_break(const char *s, size_t len, size_t cols);
    size_t wrap_find_resume(const char *s, size_t len);

**wrap_find_break** finds a suitable index to break string *s* of
length *len* aiming for a maximum line length of *cols*.

**wrap_find_resume** finds a suitable index to resume printing string
*s* of length *len* after a break. This is to skip whitespace etc.
after the break.

Both functions return an index into *s*. **wrap_find_break** returns
*len* if no suitable break location is found. **wrap_find_resume**
returns *len* if the string is empty or only trailing whitespace.

Nul characters (`'\0'`) receive no special treatment - only *len* is
used to bound the string.

Example
-------
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

Compiling
---------
Should work on Linux, BSDs, macOS, etc:

    $ make
    $ make check
    # make install [DESTDIR=] [PREFIX=/usr/local]

A CMakeLists.txt is also included.

Author
------
Sijmen J. Mulder (<ik@sjmulder.nl>). See [LICENSE.md](LICENSE.md).
