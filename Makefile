OUTS := test test2
DEPS := rsa
CC := gcc
CFLAGS := -O3 -flto -ansi -Wall
LDFLAGS := -O3 -Wall -flto
BINDIR := ~/bin
RM := rm -f

.PHONY: all
all: $(addsuffix .x, $(OUTS))

test.x: rsa.o
test2.x: rsa.o

$(addsuffix .x, $(OUTS)): %.x: %.o

%.x: %.o
	$(CC) $(LDFLAGS) $^ -o $@

$(addsuffix .o, $(DEPS)): %.o: %.h

$(OUTS): %: %.x:
	cp $^ $@

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(addsuffix .x, $(OUTS))
	$(RM) $(OUTS)

.PHONY: install
install: $(OUTS)
	mkdir -p $(BINDIR)
	install $(OUTS) $(BINDIR)
	$(RM) $(OUTS)

.PHONY: uninstall
uninstall:
	$(RM) $(addprefix $(BINDIR)/, $(OUTS))
