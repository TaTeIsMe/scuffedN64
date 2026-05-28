CXX = g++

COMMON_FLAGS = -Wall -std=c++20 -Iinclude

DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O3 -DNDEBUG

TARGET_DIR = build
TARGET = $(TARGET_DIR)/main

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,$(TARGET_DIR)/%.o,$(SRCS))
DEPENDS = $(OBJS:.o=.d)

GLFW_CFLAGS := $(shell pkg-config --cflags glfw3 gl)
GLFW_LIBS := $(shell pkg-config --libs glfw3 gl)

ifdef RELEASE
	CXXFLAGS = $(COMMON_FLAGS) $(RELEASE_FLAGS)
else
	CXXFLAGS = $(COMMON_FLAGS) $(DEBUG_FLAGS)
endif

all: $(TARGET)

-include $(DEPENDS)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(GLFW_LIBS)

$(TARGET_DIR)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GLFW_CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(DEPENDS)