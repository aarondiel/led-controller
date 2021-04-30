CC 			:= gcc
DIRS 		:= build
CFLAGS 		:= $(shell cat ./compile_flags.txt)

include ./settings.env
export

gpio: $(DIRS) build/gpio.o build/util.o build/main.o
	$(CC) $(CFLAGS) build/gpio.o build/util.o build/main.o -o $@

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIRS):
	mkdir -p $@

run:
	scp -r ./src ./include compile_flags.txt makefile settings.env $(SSH_TARGET):~/leds
	ssh -t $(SSH_TARGET) "cd leds && make && echo && ./gpio && echo"
