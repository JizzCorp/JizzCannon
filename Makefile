
all:
	rm -rf build
	mkdir build
	g++ src/main.cc -o build/jizzCannon $(shell fltk-config --ldflags) -Wall

