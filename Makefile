.PHONY: default debug install defconfig clean

CC       ?= clang
RM       = rm -f
PREFIX   ?= /usr/local
OBJDIR   = obj
SRCS     = $(wildcard src/*.c)
OBJS     = $(SRCS:src/%.c=$(OBJDIR)/%.o)
CFLAGS  += $(shell pkg-config --cflags luajit) -I./include -Wno-format-truncation -std=c99 -D_XOPEN_SOURCE=700 -Wall -Wextra 

default: pkgit

pkgit: $(OBJS)
	${CC} -o $@ $^ $(shell pkg-config --libs luajit)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	${CC} $(CFLAGS) -c -o $@ $<

debug: CFLAGS += -g -O0
debug: pkgit

install: pkgit
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit

uninstall: ${DESTDIR}${PREFIX}/bin/pkgit
	${RM} ${DESTDIR}${PREFIX}/bin/pkgit

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
