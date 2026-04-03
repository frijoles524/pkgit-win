CC       = clang++
RM       = rm -f
PREFIX   ?= /usr/local
CXXFLAGS += $(shell pkg-config --cflags --libs luajit ) -I./include

default: pkgit

pkgit: src/main.cc
	${CC} -o pkgit src/*.cc ${CXXFLAGS}
	chmod +x pkgit

install: pkgit
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit


luatest: src/lua_build.cc
	${CC} -o luatest src/lua_build.cc ${CXXFLAGS}
	chmod +x luatest

repotest: src/setup_repo.cc
	${CC} -o repotest src/setup_repo.cc ${CXXFLAGS}
	chmod +x repotest
