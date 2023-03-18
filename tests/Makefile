TARGET_EXEC := test
BUILD_DIR := ./build
SRC_DIRS := ../
CFLAGS := -ansi -Wall -Wextra -Werror -pedantic-errors -g
SRCS := $(shell find $(SRC_DIRS) -name '*.c' ! -name "*module.c" ! -name "main.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LDFLAGS := $(INC_FLAGS) -MMD -MP -lm -lcunit

all: test
# The final build step.

test: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm $(TARGET_EXEC)


-include $(DEPS)