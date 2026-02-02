
all: main.c
	gcc $< -Llib -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
