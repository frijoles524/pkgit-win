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

.PHONY: default debug install defconfig rmconfig clean uninstall

CC       ?= cc
RM       = rm -f
PREFIX   ?= /usr/local
OBJDIR   = obj
SRCS     = $(wildcard src/*.c)
OBJS     = $(SRCS:src/%.c=$(OBJDIR)/%.o)
CFLAGS  += $(shell pkg-config --cflags luajit) -I./include -Wno-format-truncation -std=c99

# pretty print for compilation. To enable verbose (show commands), run with
# `make V=1`
ifeq ($(strip $(V)),)
	E = echo
	Q = @
else
	E = :
	Q =
endif

default: pkgit

pkgit: $(OBJS)
	$(Q) echo "  LINK    " $@
	$(Q) ${CC} -o $@ $^ $(shell pkg-config --libs luajit) $(LDFLAGS) -g

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(Q) echo "  CC      " $@
	$(Q) ${CC} $(CFLAGS) -c -o $@ $<

gdb: CFLAGS += -g -O0 -Wall -Wextra -Werror -Wvla -pedantic
gdb: pkgit

debug: CFLAGS += -g -O0 -Wall -Wextra -Werror -Wvla -pedantic -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: pkgit

.PHONY: windows
windows: pkgit # package into a zip file with a cygwin root
	mkdir -p pkgit_win_portable
	cp -r win_dist/. pkgit_win_portable/
	mv pkgit.exe pkgit_win_portable/pkgit_bin.exe
	zip -r -m pkgit_win_portable.zip pkgit_win_portable/
	$(Q) echo "Compiled to pkgit_win_portable.zip"

install: pkgit
	$(Q) echo "  INSTALL " ${DESTDIR}${PREFIX}
	$(Q) install -d ${DESTDIR}${PREFIX}/bin
	$(Q) install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit
	$(Q) install -d ${DESTDIR}${PREFIX}/share/man/man1
	$(Q) install -m 644 docs/pkgit.1 ${DESTDIR}${PREFIX}/share/man/man1/pkgit.1

uninstall:
	$(Q) echo "  UNINSTALL " ${DESTDIR}${PREFIX}
	$(Q) ${RM} ${DESTDIR}${PREFIX}/bin/pkgit
	$(Q) ${RM} ${DESTDIR}${PREFIX}/share/man/man1/pkgit.1

defconfig:
	$(Q) if [ "$$(id -u)" -eq 0 ]; then dir=/etc/pkgit; else dir="$$HOME/.config/pkgit"; fi; \
	echo "installing default config to $$dir ..."; \
	mkdir -p "$$dir"; \
	cp -r ./config/* "$$dir/"; \
	echo "default config installed"

rmconfig:
	$(Q) if [ "$$(id -u)" -eq 0 ]; then dir=/etc/pkgit; else dir="$$HOME/.config/pkgit"; fi; \
	echo "removing config from $$dir ..."; \
	rm -rf "$$dir"; \
	echo "config successfully removed"

clean:
	$(Q) echo "  CLEAN"
	$(Q) ${RM} -r $(OBJDIR) pkgit
	$(Q) ${RM} pkgit_win_portable.zip