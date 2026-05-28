CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -Iinclude
TARGET_DIR = debug
TARGET = $(TARGET_DIR)/main
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, $(TARGET_DIR)/%.o, $(SRCS))
DEPENDS = $(OBJS:.o=.d)

GLFW_CFLAGS := $(shell pkg-config --cflags glfw3 gl)
GLFW_LIBS := $(shell pkg-config --libs glfw3 gl)

all: $(TARGET)

-include $(DEPENDS)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(GLFW_LIBS)

$(TARGET_DIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GLFW_CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(DEPENDS)