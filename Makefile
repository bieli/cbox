INCLUDE_DIR     = include
SRC_DIR         = src
EXAMPLES_DIR    = examples
BUILD_DIR       = build
TEST_SRC_CORE   = tests/test_cbox_core.c
TEST_BIN_CORE   = build/test_cbox_core
TEST_CBOX_SRC   = tests/test_cbox.c
TEST_CBOX_BIN   = build/test_cbox
TEST_SRC_STRUCT = tests/test_cbox_struct.c
TEST_BIN_STRUCT = build/test_cbox_struct
TEST_SRC_INTARRAY = tests/test_cbox_intarray.c
TEST_BIN_INTARRAY = build/test_cbox_intarray
TEST_SRC_CONTAINER = tests/test_cbox_container.c
TEST_BIN_CONTAINER = build/test_cbox_container
TEST_BOOL_SRC = tests/test_cbox_bool.c
TEST_BOOL_BIN = build/test_cbox_bool


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

test: all
	@echo "Running example binaries..."
	@for bin in $(EXAMPLE_BINS); do \
		if [ -x $$bin ]; then echo "Running $$bin"; $$bin || exit $$?; fi; \
	done
	@echo "All example runs succeeded."

$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c libcbox.a
	$(CC) $(CFLAGS) $< libcbox.a -o $@

test-core: $(TEST_BIN_CORE)
	@echo "Running core unit tests..."
	@./$(TEST_BIN_CORE)

$(TEST_BIN_CORE): $(TEST_SRC_CORE) libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_CORE) libcbox.a -o $(TEST_BIN_CORE)

test-struct: $(TEST_BIN_STRUCT)
	@echo "Running struct unit tests..."
	@./$(TEST_BIN_STRUCT)

$(TEST_BIN_STRUCT): $(TEST_SRC_STRUCT) libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_STRUCT) libcbox.a -o $(TEST_BIN_STRUCT)

test-cbox: $(TEST_CBOX_BIN)
	@echo "Running cbox unit tests..."
	@./$(TEST_CBOX_BIN)

$(TEST_CBOX_BIN): $(TEST_CBOX_SRC) libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_CBOX_SRC) libcbox.a -o $(TEST_CBOX_BIN)

test-bool: $(TEST_BOOL_BIN)
	@echo "Running cbox_bool unit tests..."
	@./$(TEST_BOOL_BIN)

$(TEST_BOOL_BIN): $(TEST_BOOL_SRC) libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_BOOL_SRC) libcbox.a -o $(TEST_BOOL_BIN)

test-intarray: $(TEST_BIN_INTARRAY)
	@echo "Running intarray unit tests..."
	@./$(TEST_BIN_INTARRAY)

$(TEST_BIN_INTARRAY): $(TEST_SRC_INTARRAY) libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_INTARRAY) libcbox.a -o $(TEST_BIN_INTARRAY)

test-container: $(TEST_BIN_CONTAINER)
	@echo "Running container unit tests..."
	@./$(TEST_BIN_CONTAINER)

$(TEST_BIN_CONTAINER): $(TEST_SRC_CONTAINER) libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_CONTAINER) libcbox.a -o $(TEST_BIN_CONTAINER)

test:
	@echo "Running all unit tests..."
	@echo "\n"
	@make test-cbox
	@echo "\n"
	@make test-bool
	@echo "\n"
	@make test-core
	@echo "\n"
	@make test-struct
	@echo "\n"
	@make test-intarray
	@echo "\n"
	@make test-container

clean:
	rm -rf $(BUILD_DIR) libcbox.a

.PHONY: all examples clean

