#
# BFE MAKEFILE
#
CC=icpc
INCLUDE=
FLAGS=-m64 -O2 -std=c++11
SOURCES=sha1.cpp bfe.cpp sharcbfe.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET=testbfe

all: $(TARGET)

$(OBJECTS): $(SOURCES)
	$(CC) -c $(FLAGS) $(INCLUDE) $(SOURCES)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET)

clean:
	-rm *.o
	-rm $(TARGET)
