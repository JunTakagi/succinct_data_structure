# Makefile for implicit data structure

# Macros
CC = g++

TARGET = libimplicit.so
OBJS = flexibleArray.o ptable.o
CFLAGS = -O3
LIBS = -L/opt/local/lib -lSDLmain

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -dynamiclib -o $@ $(OBJS)

clean:
	-rm -f $(TARGET) $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

flexibleArray.o:flexibleArray.hpp flexibleArray.cpp
