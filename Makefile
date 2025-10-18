INCLUDE_DIR     = include
SRC_DIR         = src
EXAMPLES_DIR    = examples
BUILD_DIR       = build
LIB_DIR         = lib
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
TEST_DATE_SRC = tests/test_cbox_date.c
TEST_DATE_BIN = build/test_cbox_date

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC -I$(INCLUDE_DIR) -g
LDFLAGS =

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

EXAMPLE_FILES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS  = $(patsubst $(EXAMPLES_DIR)/%.c,$(BUILD_DIR)/%,$(EXAMPLE_FILES))

all: $(BUILD_DIR) $(LIB_DIR) $(LIB_DIR)/libcbox.a $(LIB_DIR)/libcbox.so examples

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB_DIR)/libcbox.a: $(OBJ_FILES)
	ar rcs $@ $^

$(LIB_DIR)/libcbox.so: $(OBJ_FILES)
	$(CC) -shared -o $@ $^

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

examples: $(EXAMPLE_BINS)

test: $(TEST_BIN_CORE) $(TEST_BIN_STRUCT) $(TEST_BIN_CONTAINER) $(TEST_BIN_INTARRAY) $(TEST_BIN_BOOL) $(TEST_DATE_BIN) $(TEST_CBOX_BIN)
	@echo "Running all unit tests..."
	@./$(TEST_BIN_CORE)
	@./$(TEST_BIN_STRUCT)
	@./$(TEST_BIN_CONTAINER)
	@./$(TEST_BIN_INTARRAY)
	@./$(TEST_BIN_BOOL)
	@./$(TEST_DATE_BIN)
	@./$(TEST_CBOX_BIN)

$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) $< $(LIB_DIR)/libcbox.a -o $@

$(TEST_BIN_CORE): $(TEST_SRC_CORE) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_CORE) $(LIB_DIR)/libcbox.a -o $(TEST_BIN_CORE)

test-core: $(TEST_BIN_CORE)
	@echo "Running core unit tests..."
	@./$(TEST_BIN_CORE)

test-struct: $(TEST_BIN_STRUCT)
	@echo "Running struct unit tests..."
	@./$(TEST_BIN_STRUCT)

$(TEST_BIN_STRUCT): $(TEST_SRC_STRUCT) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_STRUCT) $(LIB_DIR)/libcbox.a -o $(TEST_BIN_STRUCT)

test-cbox: $(TEST_CBOX_BIN)
	@echo "Running cbox unit tests..."
	@./$(TEST_CBOX_BIN)

$(TEST_CBOX_BIN): $(TEST_CBOX_SRC) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_CBOX_SRC) $(LIB_DIR)/libcbox.a -o $(TEST_CBOX_BIN)

test-bool: $(TEST_BOOL_BIN)
	@echo "Running cbox_bool unit tests..."
	@./$(TEST_BOOL_BIN)

$(TEST_BOOL_BIN): $(TEST_BOOL_SRC) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_BOOL_SRC) $(LIB_DIR)/libcbox.a -o $(TEST_BOOL_BIN)

test-bool: $(TEST_DATE_BIN)
	@echo "Running cbox_date unit tests..."
	@./$(TEST_DATE_BIN)

$(TEST_DATE_BIN): $(TEST_DATE_SRC) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_DATE_SRC) $(LIB_DIR)/libcbox.a -o $(TEST_DATE_BIN)

test-intarray: $(TEST_BIN_INTARRAY)
	@echo "Running intarray unit tests..."
	@./$(TEST_BIN_INTARRAY)

$(TEST_BIN_INTARRAY): $(TEST_SRC_INTARRAY) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_INTARRAY) $(LIB_DIR)/libcbox.a -o $(TEST_BIN_INTARRAY)

test-container: $(TEST_BIN_CONTAINER)
	@echo "Running container unit tests..."
	@./$(TEST_BIN_CONTAINER)

$(TEST_BIN_CONTAINER): $(TEST_SRC_CONTAINER) $(LIB_DIR)/libcbox.a
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC_CONTAINER) $(LIB_DIR)/libcbox.a -o $(TEST_BIN_CONTAINER)

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
	rm -rf $(BUILD_DIR) $(LIB_DIR)


.PHONY: all examples clean test test-core test-struct test-cbox test-bool test-intarray test-container


