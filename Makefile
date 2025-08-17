CXX := g++
CXXFLAGS := -std=gnu++17 -O2 -Wall -Wextra -Wpedantic
INCLUDES := -Iinclude
SRC := $(wildcard src/*.cpp)
BIN_DIR := bin
TARGET := $(BIN_DIR)/expense_tracker

$(TARGET): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC)

.PHONY: clean run
clean:
	rm -rf $(BIN_DIR) *.o *.obj

run: $(TARGET)
	./$(TARGET)
