# Target executable
TARGET = test

# Compiler
CC = gcc

# Compilation options
CFLAGS = -Wall -g -ILittleD/src

# Necessary object files
OBJECTS = LittleD/bin/lib/*.o

# Build the executable
$(TARGET): $(OBJECTS) test.o
	$(CC) $(CFLAGS) -o $(TARGET) test.o $(OBJECTS)


LittleD/bin/lib/%.o: LittleD/src/dbparser/%.c LittleD/src/dbparser/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Build test object
test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

# Clean up
clean:
	rm -f $(TARGET) test.o sensors
