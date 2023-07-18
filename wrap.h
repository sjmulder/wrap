/* 
 * Finds a suitable index to break string `s` of length `len` aiming for
 * a maximum line length of `cols`. Returns `len` if no suitable break
 * was found.
 */
size_t wrap_find_break(const char *s, size_t len, size_t cols);

/*
 * Finds a suitable index to resume printing string `s` of length `len`
 * after a break. This is to skip whitespace etc. after the break.
 * Returns `len` if there's only trailing whitespace.
 */
size_t wrap_find_resume(const char *s, size_t len);
