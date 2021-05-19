CC 			:= gcc
DIRS 		:= build
CFLAGS 		:= $(shell cat ./compile_flags.txt)

include ./settings.env
export

gpio: $(dirs) build/gpio.o build/util.o build/main.o
	$(CC) $(cflags) build/gpio.o build/util.o build/main.o -o $@

test: $(dirs) build/gpio.o build/util.o build/test.o
	$(CC) $(cflags) build/gpio.o build/util.o build/test.o -o $@

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIRS):
	mkdir -p $@

copy:
	scp -r ./src ./include compile_flags.txt makefile settings.env $(SSH_TARGET):~/leds

test-remote:
	make copy
	ssh -t $(SSH_TARGET) "cd leds && make test && echo && sudo ./test && echo"

run:
	make copy
	ssh -t $(SSH_TARGET) "cd leds && make && echo && sudo ./gpio && echo"
