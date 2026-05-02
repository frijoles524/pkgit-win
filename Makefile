.PHONY: debug install defconfig clean

CC       = clang++
RM       = rm -f
PREFIX   ?= /usr/local
OBJDIR   = obj
SRCS     = $(wildcard src/*.cc)
OBJS     = $(SRCS:src/%.cc=$(OBJDIR)/%.o)
CXXFLAGS += $(shell pkg-config --cflags luajit) -I./include

default: pkgit

pkgit: $(OBJS)
	${CC} -o pkgit $^ $(shell pkg-config --libs luajit)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.cc | $(OBJDIR)
	${CC} $(CXXFLAGS) -c -o $@ $<

debug: CXXFLAGS += -g -O0
debug: pkgit

install: pkgit
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 pkgit ${DESTDIR}${PREFIX}/bin/pkgit

defconfig:
	@echo "Installing default config to ~/.config/pkgit ..."
	@mkdir -p ~/.config/pkgit
	@cp -r config/* ~/.config/pkgit
	@echo "default config installed"

clean:
	${RM} -r $(OBJDIR) pkgit