.PHONY: default debug install defconfig clean

CC       = gcc
RM       = rm -f
PREFIX   ?= /usr/local
OBJDIR   = obj
SRCS     = $(wildcard src/*.c)
OBJS     = $(SRCS:src/%.c=$(OBJDIR)/%.o)
CFLAGS  += $(shell pkg-config --cflags luajit) -I./include -Wno-format-truncation

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

defconfig:
	@echo "Installing default config to ~/.config/pkgit ..."
	@mkdir -p ~/.config/pkgit
	@cp -r ./config/* ~/.config/pkgit/
	@echo "default config installed"

clean:
	${RM} -r $(OBJDIR) pkgit
