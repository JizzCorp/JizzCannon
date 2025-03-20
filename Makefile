
all:
	g++ src/main.cc -o build/jizzCannon $(shell fltk-config --ldflags) -Wall

