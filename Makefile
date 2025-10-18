INCLUDE_DIR = include
SRC_DIR     = src
EXAMPLES_DIR = examples
BUILD_DIR   = build

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I$(INCLUDE_DIR) -g
LDFLAGS =

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

EXAMPLE_FILES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS  = $(patsubst $(EXAMPLES_DIR)/%.c,$(BUILD_DIR)/%,$(EXAMPLE_FILES))

all: $(BUILD_DIR) libcbox.a examples

libcbox.a: $(OBJ_FILES)
	ar rcs $@ $^

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

examples: $(EXAMPLE_BINS)

$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c libcbox.a
	$(CC) $(CFLAGS) $< libcbox.a -o $@

clean:
	rm -rf $(BUILD_DIR) libcbox.a

.PHONY: all examples clean

