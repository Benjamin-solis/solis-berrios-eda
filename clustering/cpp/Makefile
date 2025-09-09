CXX       := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude \
            -Wno-vla -Wno-sign-compare -Wno-unused-parameter
LDFLAGS   :=
LDLIBS    :=

SRC_DIR   := src
BUILD_DIR := build
TARGET    := ss

SOURCES   := $(wildcard $(SRC_DIR)/*.cpp) test.cpp
OBJECTS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

.PHONY: all run clean debug

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS := -std=c++17 -O0 -g -Wall -Wextra -Wpedantic -Iinclude
debug: clean all

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
