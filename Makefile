
TARGET_EXEC_MAIN := spkmeans
TARGET_EXEC_TEST := TESTspkmeans
EXES = $(TARGET_EXEC_MAIN) $(TARGET_EXEC_TEST)

BUILD_DIR := ./build
SRC_DIRS_MAIN := ./ /usr/include/python3.10
SRC_DIRS_TEST := $(SRC_DIRS_MAIN) ./test

CFLAGS := -ansi -Wall -Wextra -Werror -pedantic-errors -g
# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS_MAIN := $(shell find $(SRC_DIRS_MAIN) -name '*.c' ! -name "spkmeansmodule.c" ! -name "test.c")
SRCS_TEST := $(shell find $(SRC_DIRS_TEST) -name '*.c' ! -name "spkmeansmodule.c" ! -name "main.c")
# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS_MAIN := $(SRCS_MAIN:%=$(BUILD_DIR)/%.o)
OBJS_TEST := $(SRCS_TEST:%=$(BUILD_DIR)/%.o)
# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS_MAIN := $(OBJS_MAIN:.o=.d)
DEPS_TEST := $(OBJS_TEST:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS_MAIN := $(shell find $(SRC_DIRS_MAIN) -type d)
INC_DIRS_TEST := $(shell find $(SRC_DIRS_TEST) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS_MAIN := $(addprefix -I,$(INC_DIRS_MAIN))
INC_FLAGS_TEST := $(addprefix -I,$(INC_DIRS_TEST))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
LDFLAGS_MAIN := $(INC_FLAGS_MAIN) -MMD -MP -lm -lcunit
LDFLAGS_TEST := $(INC_FLAGS_TEST) -MMD -MP -lm -lcunit



all: $(EXES)
# The final build step.
$(EXES): $(OBJS_TEST)
	$(CC) $(OBJS_TEST) -o $@ $(LDFLAGS_TEST)
# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm $(TARGET_EXEC_MAIN)
	rm $(TARGET_EXEC_TEST)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS_TEST)