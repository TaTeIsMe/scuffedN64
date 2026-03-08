# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Target executable
TARGET = main

# For deleting the target
TARGET_DEL = main

# Source files
SRCS = $(wildcard *.cpp)

TARGET_DIR = debug

# Object files
OBJS = $(SRCS:%.cpp=$(TARGET_DIR)/%.o)

# Rule to link object files into the target executable
$(TARGET_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_DIR)/$(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
$(TARGET_DIR)/%.o: %.cpp
	mkdir -p $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm $(TARGET_DEL) $(OBJS)