BOARD ?= zkit-arm-1343
FILES  = main.c hw-timer-lpc1343.c
PORT  ?= ttyUSB0

all:
	zccc $(BOARD) $(FILES)

flash:
	sudo lpc21isp -control -bin ./out.bin /dev/$(PORT) 115200 12000
