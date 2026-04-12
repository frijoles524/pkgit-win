CC       = clang++
RM       = rm -f
PREFIX   ?= /usr/local
CXXFLAGS += $(shell pkg-config --cflags --libs luajit ) -I./include -L/usr/lib

default: pkgit

pkgit: src/main.cc
	${CC} -o pkgit src/*.cc ${CXXFLAGS}
	chmod +x pkgit

install: pkgit
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit
