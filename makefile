CC=gcc
CCFLAGS=-Wall -Wextra -pedantic
LDFLAGS=
SRCDIR=src
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=main

DBGDIR=debug
DBGBIN=$(DBGDIR)/$(TARGET)
DBGOBJS=$(addprefix $(DBGDIR)/, $(notdir $(OBECTS)))
DBGCCLFAGS=-g -O0 

RELDIR=release
RELBIN=$(RELDIR)/$(TARGET)
RELOBJS = $(addprefix $(RELDIR)/, $(notdir $(OBJECTS)))
RELCCFLAGS = -O3

.PHONY: all clean debug prep release remake

all: prep release

debug: $(DBGBIN)

$(DBGBIN): $(DBGOBJS)
	$(CC) $(CCFLAGS) $(DBGCCFLAGS) -o $(DBGBIN) $^

$(DBGDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CCFLAGS) $(DBGFLAGS) -o $@ $<

release: $(RELBIN)

$(RELBIN): $(RELOBJS)
	$(CC) $(CCFLAGS) $(RELCCFLAGS) -o $(RELBIN) $^

$(RELDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CCFLAGS) $(RELCCFLAGS) -o $@ $<

prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELBIN) $(RELOBJS) $(DBGBIN) $(DBGOBJS)

