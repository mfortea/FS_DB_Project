# Compiler
CXX = g++
CC = gcc

# Compilation options
CXXFLAGS = -Wall -g -I/usr/include/mysql -I/usr/include/curl
CFLAGS = -Wall -g -I/usr/include/mysql -I/usr/include/curl

# Linker options
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lcurl

# Build the test executable
test: test.o LittleD/bin/lib/*.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

LittleD/bin/lib/%.o: LittleD/src/dbparser/%.c LittleD/src/dbparser/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Build the benchmark executable
benchmark: benchmark.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Build test object
test.o: test.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build benchmark object
benchmark.o: benchmark.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f test benchmark *.o sensors
