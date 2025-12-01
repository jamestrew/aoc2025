CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

UTIL_OBJ = $(BUILD_DIR)/util.o

DAY_SOURCES = $(wildcard $(SRC_DIR)/day*.c)
DAY_BINS = $(patsubst $(SRC_DIR)/day%.c,$(BIN_DIR)/day%,$(DAY_SOURCES))

.PHONY: all clean

all: $(DAY_BINS)

gen%:
	./scripts/gen_day.sh $*

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/day%: $(BUILD_DIR)/day%.o $(UTIL_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

run%: $(BIN_DIR)/day%
	./$<

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
