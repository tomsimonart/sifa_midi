CC = gcc
CFLAGS = -Wall -Wextra -I$(IDIR)
LDFLAGS = -L$(LDIR)
LDLIBS = 

SDIR = src
IDIR = include
LDIR = lib
ODIR = build
BDIR = bin

PROG = $(BDIR)/sifa_midi

_OBJS = main.o
OBJS = $(_OBJS:%.o=$(ODIR)/%.o)

.PHONY: all
all: $(ODIR) $(BDIR) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^

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
