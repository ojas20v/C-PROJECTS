CC = gcc
CFLAGS =

SOURCES = main.c linkedlist.c
OBJECTS = $(SOURCES:.c=.o)
EXEC = rental_app

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)