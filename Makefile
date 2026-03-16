CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude

SRC_DIR = src
EXAMPLE_DIR = examples
BUILD_DIR = build
BIN_DIR = bin

LIB_SRC = $(wildcard $(SRC_DIR)/*.cpp)
LIB_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRC))

EXAMPLE_SRC = $(wildcard $(EXAMPLE_DIR)/*.cpp)
EXAMPLE_BIN = $(patsubst $(EXAMPLE_DIR)/%.cpp,$(BIN_DIR)/%,$(EXAMPLE_SRC))

all: $(EXAMPLE_BIN)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(EXAMPLE_DIR)/%.cpp $(LIB_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJ) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
	for f in $(BIN_DIR)/*; do \
		echo "================================"; \
		echo "Running $$f"; \
		echo "================================"; \
		$$f; \
	done

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
.SECONDARY: $(LIB_OBJ)