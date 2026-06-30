#  pkgit - package it!

#  Copyright (C) 2026 dacctal
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

.PHONY: default debug install defconfig clean

CC       ?= clang
RM       = rm -f
PREFIX   ?= /usr/local
OBJDIR   = obj
SRCS     = $(wildcard src/*.c)
OBJS     = $(SRCS:src/%.c=$(OBJDIR)/%.o)
CFLAGS  += $(shell pkg-config --cflags luajit) -I./include -Wno-format-truncation -std=c99 -D_XOPEN_SOURCE=700 

default: pkgit

pkgit: $(OBJS)
	${CC} -o $@ $^ $(shell pkg-config --libs luajit)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	${CC} $(CFLAGS) -c -o $@ $<

windows: default
	@echo "Building pkgit for windows. May be unstable! Use with caution"
	@rm -f pkgit_win_portable.zip
	@cp win_template.zip pkgit_win_portable.zip
	@echo "Adding exe to distribution"
	@zip -j pkgit_win_portable.zip pkgit.exe
	@rm -f pkgit.exe
	@$(MAKE) clean
	@echo "Done. Packaged to pkgit_win_portable.zip"

debug: CFLAGS += -g -O0
debug: pkgit

install: pkgit
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit
	install -d ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 docs/pkgit.1 ${DESTDIR}${PREFIX}/share/man/man1/pkgit.1

uninstall: ${DESTDIR}${PREFIX}/bin/pkgit
	${RM} ${DESTDIR}${PREFIX}/bin/pkgit
	${RM} ${DESTDIR}${PREFIX}/share/man/man1/pkgit.1

defconfig:
	@if [ "$$(id -u)" -eq 0 ]; then dir=/etc/pkgit; else dir=~/.config/pkgit; fi; \
	echo "installing default config to $$dir ..."; \
	mkdir -p $$dir; \
	cp -r ./config/* $$dir/; \
	echo "default config installed"

rmconfig:
	@if [ "$$(id -u)" -eq 0 ]; then dir=/etc/pkgit; else dir=~/.config/pkgit; fi; \
	echo "removing config from $$dir ..."; \
	rm -rf $$dir; \
	echo "config successfully removed"

clean:
	${RM} -r $(OBJDIR) pkgit
