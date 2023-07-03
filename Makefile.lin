CXX = g++
CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -g
DEPFLAGS = -MMD -MP
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

SRC_DIR = ./src
BUILD_DIR = ./build

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

BINPATH = ./game

all: $(BINPATH)

$(BINPATH): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

.PHONY: all clean
clean:
	rm -f $(BINPATH)
	rm -rf $(BUILD_DIR)

-include $(DEPS)
