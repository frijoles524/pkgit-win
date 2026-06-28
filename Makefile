.PHONY: default debug install defconfig clean

CC       ?= clang
RM       = rm -f
PREFIX   ?= /usr/local
OBJDIR   = obj
SRCS     = $(wildcard src/*.c)
OBJS     = $(SRCS:src/%.c=$(OBJDIR)/%.o)
CFLAGS  += $(shell pkg-config --cflags luajit) -I./include -Wno-format-truncation -std=c99 -D_XOPEN_SOURCE=700 -Wall -Wextra 

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
	$(Q) ${CC} -o $@ $^ $(shell pkg-config --libs luajit)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(E) "  CC      " $@
	$(Q) ${CC} $(CFLAGS) -c -o $@ $<

debug: CFLAGS += -g -O0
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
