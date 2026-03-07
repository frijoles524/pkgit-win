CC      = clang++
RM      = rm -f
PREFIX  ?= /usr/local
CXXFLAGS = $(shell pkg-config --cflags --libs luajit )

default: pkgit

pkgit: src/main.cc
	$(CC) -o pkgit src/main.cc -I"./include" $(CXXFLAGS)
	chmod +x pkgit

install: pkgit
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 pkgit $(DESTDIR)$(PREFIX)/bin/pkgit
