DESTDIR?=
PREFIX?=	/usr/local
MANPREFIX?=	${PREFIX}/man

INSTALL?=	install
RM?=		rm -f

CFLAGS+=	-Wall -Wextra

all: wrap sample test libwrap.a

clean:
	${RM} wrap sample test libwrap.a *.o

check: all
	./test
	./sample | diff -u - sample-out.txt
	./wrap <sample-in.txt | diff -u - sample-out.txt

lint:
	mandoc -Tlint wrap.1
	mandoc -Tlint libwrap.3

install: all
	${INSTALL} -d ${DESTDIR}${PREFIX}/bin \
	              ${DESTDIR}${PREFIX}/lib \
	              ${DESTDIR}${PREFIX}/include \
	              ${DESTDIR}${MANPREFIX}/man1 \
	              ${DESTDIR}${MANPREFIX}/man3
	${INSTALL} -m755 wrap      ${DESTDIR}${PREFIX}/bin/
	${INSTALL} -m755 wrap.h    ${DESTDIR}${PREFIX}/include/
	${INSTALL} -m755 libwrap.a ${DESTDIR}${PREFIX}/lib/
	${INSTALL} -m755 wrap.1    ${DESTDIR}${MANPREFIX}/man1/
	${INSTALL} -m755 libwrap.3 ${DESTDIR}${MANPREFIX}/man3/
	(cd ${DESTDIR}${MANPREFIX}/man3 && \
	 ln -s libwrap.3 wrap_find_break.3 && \
	 ln -s libwrap.3 wrap_find_resume.3)

uninstall:
	${RM} ${DESTDIR}${PREFIX}/bin/wrap \
	      ${DESTDIR}${PREFIX}/lib/libwrap.a \
	      ${DESTDIR}${PREFIX}/include/wrap.h \
	      ${DESTDIR}${MANPREFIX}/man1/wrap.1 \
	      ${DESTDIR}${MANPREFIX}/man3/libwrap.3 \
	      ${DESTDIR}${MANPREFIX}/man3/wrap_find_break.3 \
	      ${DESTDIR}${MANPREFIX}/man3/wrap_find_resume.3

wrap: main.o libwrap.a
	${LINK.c} -L. -o wrap main.o -lwrap

sample: sample.o libwrap.a
	${LINK.c} -L. -o sample sample.o -lwrap

test: test.o libwrap.a
	${LINK.c} -L. -o test test.o -lwrap

libwrap.a: wrap.o
	${AR} ${ARFLAGS} libwrap.a wrap.o

main.o:   wrap.h
sample.o: wrap.h
test.o:   wrap.h
wrap.o:   wrap.h

.PHONY: all clean check lint install uninstall
