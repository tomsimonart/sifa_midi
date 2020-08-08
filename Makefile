CC = gcc
CFLAGS = -Wall -Wextra -I$(IDIR)
LDFLAGS = -L$(LDIR)
LDLIBS = -lportmidi -lhidapi

SDIR = src
IDIR = include
LDIR = lib
ODIR = build
BDIR = dist

PROG = $(BDIR)/tswctrl

_OBJS = main.o sifa.o kbd_win.o joystick.o
OBJS = $(_OBJS:%.o=$(ODIR)/%.o)

.PHONY: all
all: $(ODIR) $(BDIR) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

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
