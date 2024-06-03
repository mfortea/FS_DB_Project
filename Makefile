# Compiler
CXX = g++
CC = gcc

CXXFLAGS = -Wall -g -I/usr/include/mysql -I/usr/include/curl -I/usr/local/include/mongocxx -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lcurl -lmongocxx -lbsoncxx
CFLAGS = -Wall -g -ILittleD/src -ILittleD/src/include

# Source files
SOURCES_CPP = benchmark.cpp
SOURCES_C = littled.c

# Object files
OBJECTS_CPP = $(SOURCES_CPP:.cpp=.o)
OBJECTS_C = $(SOURCES_C:.c=.o)

# Target names
TARGET_CPP = benchmark
TARGET_C = littled

# Build the littled executable
$(TARGET_C): $(OBJECTS_C) LittleD/bin/lib/*.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

LittleD/bin/lib/%.o: LittleD/src/dbparser/%.c LittleD/src/dbparser/%.h
	$(CC) $(CFLAGS) -c $< -o $@


# Build the benchmark executable
$(TARGET_CPP): $(OBJECTS_CPP)
	$(CXX) $(OBJECTS_CPP) -o $@ $(LDFLAGS)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build all targets
all: $(TARGET_C) $(TARGET_CPP)

clean:
	rm -f $(TARGET_C) $(TARGET_CPP) *.o sensors results.txt
