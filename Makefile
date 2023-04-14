
TARGET_EXEC := spkmeans
CC := gcc
BUILD_DIR := ./build
SRC_DIRS := ./
SRC_DIRS_EXCLUDE := $(SRC_DIRS) -name '*.c' ! -name '*module.c' ! -path "./tests/*"
CFLAGS := -ansi -Wall -Wextra -Werror -pedantic-errors -O3
# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS_EXCLUDE))
# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
#INC_DIRS := $(shell find $(INC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
#INC_FLAGS := $(addprefix -I,$(INC_DIRS))
INC_FLAGS := -I/usr/include/python3
# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
LDFLAGS := $(INC_FLAGS) -MMD -MP -lm -lcunit



all: $(TARGET_EXEC)
# The final build step.

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm $(TARGET_EXEC)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)