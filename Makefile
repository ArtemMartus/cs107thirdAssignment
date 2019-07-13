CC = gcc
CFLAGS = -g -c -Wall
LDFLAGS = 
SRC = main.c vector.c hashtable.c
OBJ = main.o vector.o hashtable.o
FILES = $(SRC) vector.h hashtable.h
NAME = c_containers

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

$(OBJ): $(FILES)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm *o $(NAME)