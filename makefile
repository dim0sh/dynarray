CC := gcc

CFLAGS := -std=c99 -Wall -O3

BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS = src/arraylist
OBJ := $(SRCS:%=$(BUILD_DIR)/%.o)


$(BUILD_DIR)/arraylist: $(OBJ)
	$(CC) -s -shared -o $@.dll $(OBJ) -I.$(SRC_DIRS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -D ARRAYLIST_EXPORTS

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
