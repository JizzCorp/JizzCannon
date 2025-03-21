CC = g++
CFLAGS = -Wall

all:
	rm -rf build
	mkdir build
	# g++ src/main.cc -o build/jizzCannon $(shell fltk-config --ldflags) -Wall
	$(CC) src/main.cpp src/jizzCannonEditor/jizzCannon.cpp -o ./build/jizzCannon $(CFLAGS) $(shell fltk-config --ldflags)

