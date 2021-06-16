CC 			:= gcc
CFLAGS 		:= $(shell cat ./compile_flags.txt)

include ./settings.env
export

controller: build/main.o build/bcm2835.o build/util.o
	$(CC) $(CFLAGS) $^ -o $@

test: build/test.o build/bcm2835.o build/util.o
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: copy
copy:
	scp -r ./src ./include compile_flags.txt makefile settings.env $(SSH_TARGET):~/led-controller

.PHONY: test-remote
test-remote:
	make copy
	ssh -t $(SSH_TARGET) "cd led-controller && make test && echo && sudo ./test && echo"

.PHONY: run
run:
	make copy
	ssh -t $(SSH_TARGET) "cd led-controller && make && echo && sudo ./controller && echo"

.PHONY: clean
clean:
	rm -r build
