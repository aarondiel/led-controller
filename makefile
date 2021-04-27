CC 			:= gcc
BUILD 		:= ./build
SRC 		:= ./src
GPIO_OBJ 	:= gpio.o

include ./settings.env
export

gpio: $(BUILD)/gpio.o
	$(CC) $(BUILD)/gpio.o -o $@

$(BUILD)/gpio.o: $(SRC)/gpio.c
	$(CC) -c $(SRC)/gpio.c -o $(BUILD)/gpio.o

run:
	scp -r ./src makefile settings.env $(SSH_TARGET):~/leds
	ssh -t aaron@192.168.150.54 "cd leds && make && echo && ./gpio && echo"
