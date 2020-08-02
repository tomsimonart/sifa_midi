CC = gcc
CFLAGS = -Wall -Wextra -I$(IDIR)
LDFLAGS = -L$(LDIR)
LDLIBS = -lportmidi

SDIR = src
IDIR = include
LDIR = lib
ODIR = build
BDIR = dist

PROG = $(BDIR)/sifa_midi

_OBJS = main.o kbd_win.o
OBJS = $(_OBJS:%.o=$(ODIR)/%.o)

.PHONY: all
all: $(ODIR) $(BDIR) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	cp $(LDIR)/*.dll $(BDIR)/

$(OBJS): $(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(ODIR) $(BDIR):
	mkdir $@

.PHONY: clean
clean:
	-$(RM) $(ODIR)/*

.PHONY: run
run: all
	-@./$(PROG)
