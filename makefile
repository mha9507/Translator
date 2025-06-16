# makefile
# Makefile for compiling the translator program.
# Defines rules to build the executable from source files.

# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -g -std=c++14  # Updated to c++14 for lambda auto parameters

# Target executable name
TARGET = translator

# Source files
SOURCES = main.cpp hashtable.cpp dictionary.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = hashtable.h dictionary.h

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean