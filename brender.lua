#!/usr/bin/env lua
os.execute("echo '[***BUILDING RENDER LIB***]'")
os.execute("gcc render.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")

os.execute("echo '[***LINKING***]'")
os.execute(
	"gcc -o game main.o render.o game.o -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
