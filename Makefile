CC       = clang++
RM       = rm -f
PREFIX   ?= /usr/local
CXXFLAGS += $(shell pkg-config --cflags --libs luajit ) -I./include -L/usr/lib

default: pkgit

pkgit: src/*.cc
	${CC} -o pkgit src/*.cc ${CXXFLAGS}

debug: src/*.cc
	${CC} -o pkgit src/*.cc ${CXXFLAGS} -g -O0

install: pkgit
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit
