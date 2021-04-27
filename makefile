CC 			:= gcc
BUILD 		:= ./build
SRC 		:= ./src
DIRS 		:= $(BUILD) $(SRC)

include ./settings.env
export

gpio: $(DIRS) $(BUILD)/gpio.o
	$(CC) $(BUILD)/gpio.o -o $@

$(BUILD)/gpio.o: $(SRC)/gpio.c
	$(CC) -c $(SRC)/gpio.c -o $(BUILD)/gpio.o

$(DIRS):
	mkdir -p $@

run:
	scp -r ./src makefile settings.env $(SSH_TARGET):~/leds
	ssh $(SSH_TARGET) "cd leds && make && echo && ./gpio && echo"

permissions:
	sudo sh -c "chgrp gpio /dev/gpiochip* && chmod g+rw /dev/gpiochip*"
