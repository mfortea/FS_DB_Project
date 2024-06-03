# Variables
CXX = g++
CC = cc
CXXFLAGS = -Wall -g -I/usr/include/mysql -I/usr/include/curl -I/usr/local/include/mongocxx -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lcurl -lmongocxx -lbsoncxx

# Archivos fuente y objeto
SOURCES_CPP = benchmark.cpp
OBJECTS_CPP = $(SOURCES_CPP:.cpp=.o)

# Ejecutable
TARGET = benchmark

# Regla por defecto
all: $(TARGET)

# Regla para el ejecutable
$(TARGET): $(OBJECTS_CPP)
	$(CXX) $(OBJECTS_CPP) -o $@ $(LDFLAGS)

# Reglas para los archivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar
clean:
	rm -f test benchmark *.o sensors
