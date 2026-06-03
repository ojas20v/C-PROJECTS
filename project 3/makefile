# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -Werror

# Executable name
TARGET = spam_detector

# Source files
SRC = main.c

# Build the program
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the program
program: $(TARGET)
	./$(TARGET)

# Clean compiled files
clean:
	rm -f $(TARGET)