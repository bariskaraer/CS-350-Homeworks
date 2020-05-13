CC=gcc
DEPS=meal.h
LDFLAGS=-pthread
default: kitchen

kitchenmake: kitchen.c meal.h
	@gcc -w -pthread -o kitchen kitchen.c
	
clean:
	@rm -f	kitchen.o
