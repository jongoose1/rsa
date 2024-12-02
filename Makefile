OUTS := encrypt decrypt keygen cppk print_keypair print_public_key sign verify
INSTALLDIR := ~/bin

.PHONY: all
all: debug release test $(OUTS)

.PHONY: debug
debug: $(addprefix debug/, $(addsuffix .x, $(OUTS)))
debug/%.x: debug/%.o debug/rsa.o
	gcc -Wall $^ -o $@
debug/rsa.o: src/rsa.h
debug/%.o: src/%.c | ddir
	gcc -ansi -Wall -g -c $< -o $@
.PHONY: ddir
ddir:
	mkdir -p debug

.PHONY: release
release: $(addprefix release/, $(addsuffix .x, $(OUTS)))
release/%.x: release/%.o release/rsa.o
	gcc -Wall -O3 -flto $^ -o $@
release/rsa.o: src/rsa.h
release/%.o: src/%.c | rdir
	gcc -ansi -w -DNDEBUG -O3 -flto -c $< -o $@
.PHONY: rdir
rdir:
	mkdir -p release

.PHONY: test
test: $(addsuffix .x, $(basename $(wildcard test/*.c)))
test/%.x: test/%.o debug/rsa.o
	gcc -Wall $^ -o $@
test/%.o: test/%.c
	gcc -ansi -Wall -g -c $< -o $@

$(OUTS): %: release/%.x
	cp $< $@

.PHONY: clean
clean:
	rm -rf debug
	rm -rf release
	rm -f $(OUTS)
	rm -f test/*.x

.PHONY: install
install: $(OUTS)
	mkdir -p $(INSTALLDIR)
	install $(OUTS) $(INSTALLDIR)

.PHONY: uninstall
uninstall:
	rm -f $(addprefix $(INSTALLDIR)/, $(OUTS))
