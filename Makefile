#janky ass makefile

OUTS := mod_exp_test miller_rabin_test gcd_test mod_test prime_finder div_test \
lcm_test bezout_test prime_tester incremental_prime_finder test io_test \
encrypt_test encrypt decrypt keygen
DEPS := rsa
CC := gcc
INSTALLDIR := ~/bin
CFLAGS := -ansi -Wall
LDFLAGS := -Wall
RM := rm -f

.PHONY: all
all: debug release $(OUTS)

.PHONY: debug
debug: $(addprefix debug/bin/, $(addsuffix .x, $(OUTS)))
$(addprefix debug/bin/, $(addsuffix .x, $(OUTS))): debug/bin/%.x: debug/obj/%.o debug/obj/rsa.o
debug/bin/%.x: debug/obj/%.o
	$(CC) $(LDFLAGS) $^ -o $@
$(addprefix debug/obj/, $(addsuffix .o, $(DEPS))): debug/obj/%.o: src/%.h
debug/obj/%.o: src/%.c | ddirs
	$(CC) $(CFLAGS) -g -c $< -o $@
.PHONY: ddirs
ddirs:
	mkdir -p debug
	mkdir -p debug/bin
	mkdir -p debug/obj

.PHONY: release
release: $(addprefix release/bin/, $(addsuffix .x, $(OUTS)))
$(addprefix release/bin/, $(addsuffix .x, $(OUTS))): release/bin/%.x: release/obj/%.o release/obj/rsa.o
release/bin/%.x: release/obj/%.o
	$(CC) $(LDFLAGS) -O3 -flto -DNDEBUG $^ -o $@
$(addprefix release/obj/, $(addsuffix .o, $(DEPS))): release/obj/%.o: src/%.h
release/obj/%.o: src/%.c | rdirs
	$(CC) $(CFLAGS) -O3 -flto -c $< -o $@
.PHONY: rdirs
rdirs:
	mkdir -p release
	mkdir -p release/bin
	mkdir -p release/obj

$(OUTS): %: release/bin/%.x
	cp $< $@

.PHONY: clean
clean:
	$(RM) -r debug
	$(RM) -r release
	$(RM) $(OUTS)

.PHONY: install
install: $(OUTS)
	mkdir -p $(INSTALLDIR)
	install $(OUTS) $(INSTALLDIR)

.PHONY: uninstall
uninstall:
	$(RM) $(addprefix $(INSTALLDIR)/, $(OUTS))
