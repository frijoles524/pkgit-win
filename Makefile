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

# pretty print for compilation. To enable verbose (show commands), run with
# `make V=1`
ifeq ($(strip $(V)),)
	E = @echo
	Q = @
else
	E = @\#
	Q =
endif

default: pkgit

pkgit: $(OBJS)
	$(E) "  LINK    " $@
	$(Q) ${CC} -o $@ $^ $(shell pkg-config --libs luajit) $(LDFLAGS)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(E) "  CC      " $@
	$(Q) ${CC} $(CFLAGS) -c -o $@ $<

debug: CFLAGS += -g -O0 -Wall -Wextra -Werror -Wvla -pedantic -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: pkgit

install: pkgit
	$(E)  "  INSTALL" ${DESTDIR}${PREFIX}
	$(Q) install -d ${DESTDIR}${PREFIX}/bin
	$(Q) install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit
	$(Q) install -d ${DESTDIR}${PREFIX}/share/man/man1
	$(Q) install -m 644 docs/pkgit.1 ${DESTDIR}${PREFIX}/share/man/man1/pkgit.1

uninstall: ${DESTDIR}${PREFIX}/bin/pkgit
	$(E)  "  UNINSTALL" ${DESTDIR}${PREFIX}
	$(Q) ${RM} ${DESTDIR}${PREFIX}/bin/pkgit
	$(Q) ${RM} ${DESTDIR}${PREFIX}/share/man/man1/pkgit.1
 
defconfig:
	$(Q) @if [ "$$(id -u)" -eq 0 ]; then dir=/etc/pkgit; else dir=~/.config/pkgit; fi; \
	$(Q) echo "installing default config to $$dir ..."; \
	$(Q) mkdir -p $$dir; \
	$(Q) cp -r ./config/* $$dir/; \
	$(Q) echo "default config installed"
 
rmconfig:
	$(Q) @if [ "$$(id -u)" -eq 0 ]; then dir=/etc/pkgit; else dir=~/.config/pkgit; fi; \
	$(Q) echo "removing config from $$dir ..."; \
	$(Q) rm -rf $$dir; \
	$(Q) echo "config successfully removed"
 
clean:
	$(E) "  CLEAN"
	$(Q) ${RM} -r $(OBJDIR) pkgit
