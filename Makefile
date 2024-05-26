OUTS := mod_exp_test miller_rabin_test gcd_test mod_test prime_finder div_test lcm_test bezout_test
DEPS := rsa
CC := gcc
CFLAGS := -O3 -flto -ansi -Wall
LDFLAGS := -O3 -Wall -flto
BINDIR := ~/bin
RM := rm -f

.PHONY: all
all: $(addsuffix .x, $(OUTS))

prime_finder.x: rsa.o
mod_exp_test.x: rsa.o
mod_test.x: rsa.o
div_test.x: rsa.o
lcm_test.x: rsa.o
bezout_test.x: rsa.o
gcd_test.x: rsa.o
miller_rabin_test.x: rsa.o

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
