CC = gcc
CFLAGS = -Wall -Wextra -Wconversion -g -fPIC
RELEASE_CFLAGS = -Wall -Wextra -Wconversion -O2 -DNDEBUG -fPIC
LDFLAGS = -ldl -lm
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

UTIL_OBJ = $(BUILD_DIR)/util.o
DAY_SOURCES = $(filter-out $(SRC_DIR)/day0.c $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/day[0-9]*.c))
DAY_LIBS = $(patsubst $(SRC_DIR)/day%.c,$(BIN_DIR)/libday%.so,$(DAY_SOURCES))

.PHONY: all clean

all: aoc $(DAY_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/libday%.so: $(BUILD_DIR)/day%.o $(UTIL_OBJ) | $(BIN_DIR)
	$(CC) -shared $^ -o $@ -lm

aoc: $(BUILD_DIR)/main.o $(UTIL_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

day%: $(SRC_DIR)/day%.c $(UTIL_OBJ) | $(BIN_DIR)
	$(CC) $(RELEASE_CFLAGS) $< $(UTIL_OBJ) -o $(BIN_DIR)/$@ -lm

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) aoc
