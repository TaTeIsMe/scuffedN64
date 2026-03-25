CXX = g++
CXXFLAGS = -Wall -g
TARGET_DIR = debug
TARGET = $(TARGET_DIR)/main
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=$(TARGET_DIR)/%.o)
DEPENDS := $(SRCS:%.cpp=$(TARGET_DIR)/%.d)

all: $(TARGET)

-include $(DEPENDS)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(TARGET_DIR)/%.o: %.cpp
	mkdir -p $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(DEPENDS)