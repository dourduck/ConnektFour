#!/usr/bin/env lua
os.execute("echo '[***BULIDING GAME LIB***]'")
os.execute("gcc game.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")

os.execute("echo '[***BUILDING RENDER LIB***]'")
os.execute("gcc render.c -c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")

os.execute("echo '[***BUILDING MAIN***]'")
os.execute("gcc -c main.c -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")

os.execute("echo '[***LINKING***]'")
os.execute(
	"gcc -o game main.o game.o render.o -I./include/ -L./lib/ -lraylib -Wl,-rpath,$(pwd)/lib/ -lm -lpthread -ldl -lrt -lX11")
