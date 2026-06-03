# Compiler to use
CC = gcc

# Compiler flags (optional: -g for debugging, -Wall for all warnings)
CFLAGS = -g -Wall

# Target name
TARGET = program

# Source file
SRC = main.c

# Build rules
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean rule to remove the compiled program
clean:
	rm -f $(TARGET)
