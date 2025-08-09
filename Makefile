# Cross-platform Makefile for GSDK

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	SHARED_EXT = dylib
	SHARED_FLAGS = -dynamiclib -install_name @rpath/$(@F)
	RPATH_FLAGS = -Wl,-rpath,@executable_path/../lib
	LDFLAGS += -lpthread -lm
else
	SHARED_EXT = so
	SHARED_FLAGS = -shared -Wl,-soname,$(@F)
	RPATH_FLAGS = -Wl,-rpath,'$$ORIGIN/../lib'
	LDFLAGS += -lpthread -pthread -O3
endif

# Compiler
CC = clang #/opt/gcc-15.1.0/bin/gcc
CFLAGS = -fPIC -Wall -Wextra -O2 -Iinclude -Iinclude/core -Iinclude/data -Iinclude/performance -Iinclude/reflection -std=c23 -g

# Directories
SRC_DIR      = src
BUILD_DIR    = build
BUILD_LIB_DIR     = $(BUILD_DIR)/lib
BUILD_EXAMPLE_DIR = $(BUILD_DIR)/examples
BUILD_TEST_DIR    = $(BUILD_DIR)/tests
BUILD_UTIL_DIR    = $(BUILD_DIR)/utilities
EXAMPLES_DIR = $(SRC_DIR)/examples
TESTS_DIR    = $(SRC_DIR)/test
UTILS_DIR    = $(SRC_DIR)/utilities
ROOT_DIR     = $(shell pwd)

# Core libraries
CORE_LIBS = interfaces log sync pack rsa sha hash socket stream
DATA_LIBS = adjacency_list adjacency_matrix array bitmap cache circular_buffer dict b binary tuple priority_queue queue stack red_black
REFLECTION_LIBS = base64 json
PERFORMANCE_LIBS = parallel

# Lists of targets
LIBS = $(CORE_LIBS) $(DATA_LIBS) $(REFLECTION_LIBS) $(PERFORMANCE_LIBS)
TESTS = $(DATA_LIBS) $(REFLECTION_LIBS)
UTILS = rsa_key_generator rsa_key_info hash_optimal lisp_syntax_highlighter aes_assert secure_socket_client secure_socket_server

# Phony targets
.PHONY: all clean libs examples utils tests

#############
# Libraries #
#############
all: libs examples utils tests main

main: main.c $(addprefix $(BUILD_LIB_DIR)/, $(addsuffix .$(SHARED_EXT), $(LIBS)))
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o build/main main.c $(addprefix $(ROOT_DIR)/$(BUILD_LIB_DIR)/, $(addsuffix .$(SHARED_EXT), $(LIBS)))

libs: $(addprefix $(BUILD_LIB_DIR)/, $(addsuffix .$(SHARED_EXT), $(LIBS)))

$(BUILD_LIB_DIR):
	@mkdir -p $@

# Core
$(BUILD_LIB_DIR)/log.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/log/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/sync.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/sync/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/pack.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/pack/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/rsa/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/sha.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/sha/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/hash.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/hash/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/socket.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/socket/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/interfaces/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_LIB_DIR)/stream.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/stream/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/socket.$(SHARED_EXT)

# Data structures
$(BUILD_LIB_DIR)/adjacency_list.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/adjacency_list/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/adjacency_matrix.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/adjacency_matrix/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/array.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/array/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/bitmap/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/cache.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/cache/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/circular_buffer/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/dict.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/dict/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/binary.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/binary/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/b.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/b/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)	

$(BUILD_LIB_DIR)/tuple.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/tuple/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/priority_queue/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/queue/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/stack.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/stack/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/red_black.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/red_black/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# Reflection
$(BUILD_LIB_DIR)/base64.$(SHARED_EXT): $(wildcard $(SRC_DIR)/reflection/base64/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/json.$(SHARED_EXT): $(wildcard $(SRC_DIR)/reflection/json/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ -lm $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# Performance
$(BUILD_LIB_DIR)/parallel.$(SHARED_EXT): $(wildcard $(SRC_DIR)/performance/parallel/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

############
# Examples #
############
examples: $(addprefix $(BUILD_EXAMPLE_DIR)/, $(addsuffix _example, $(LIBS)))

$(BUILD_EXAMPLE_DIR):
	@mkdir -p $@

$(BUILD_EXAMPLE_DIR)/log_example: $(EXAMPLES_DIR)/log_example.c $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/sync_example: $(EXAMPLES_DIR)/sync_example.c $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/pack_example: $(EXAMPLES_DIR)/pack_example.c $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/rsa_example: $(EXAMPLES_DIR)/rsa_example.c $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/sha_example: $(EXAMPLES_DIR)/sha_example.c $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/stream_example: $(EXAMPLES_DIR)/stream_example.c $(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/socket.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/interfaces_example: $(EXAMPLES_DIR)/interfaces_example.c $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/hash_example: $(EXAMPLES_DIR)/hash_example.c $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/socket_example: $(EXAMPLES_DIR)/socket_example.c $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/array_example: $(EXAMPLES_DIR)/array_example.c $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/adjacency_list_example: $(EXAMPLES_DIR)/adjacency_list_example.c $(BUILD_LIB_DIR)/adjacency_list.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/adjacency_matrix_example: $(EXAMPLES_DIR)/adjacency_matrix_example.c $(BUILD_LIB_DIR)/adjacency_matrix.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/bitmap_example: $(EXAMPLES_DIR)/bitmap_example.c $(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/cache_example: $(EXAMPLES_DIR)/cache_example.c $(BUILD_LIB_DIR)/cache.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/circular_buffer_example: $(EXAMPLES_DIR)/circular_buffer_example.c $(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/dict_example: $(EXAMPLES_DIR)/dict_example.c $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/binary_example: $(EXAMPLES_DIR)/binary_example.c $(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/b_example: $(EXAMPLES_DIR)/b_example.c $(BUILD_LIB_DIR)/b.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)	 | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/tuple_example: $(EXAMPLES_DIR)/tuple_example.c $(BUILD_LIB_DIR)/tuple.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/priority_queue_example: $(EXAMPLES_DIR)/priority_queue_example.c $(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/queue_example: $(EXAMPLES_DIR)/queue_example.c $(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/stack_example: $(EXAMPLES_DIR)/stack_example.c $(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/red_black_example: $(EXAMPLES_DIR)/red_black_example.c $(BUILD_LIB_DIR)/red_black.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/base64_example: $(EXAMPLES_DIR)/base64_example.c $(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/json_example: $(EXAMPLES_DIR)/json_example.c $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ -lm

$(BUILD_EXAMPLE_DIR)/parallel_example: $(EXAMPLES_DIR)/parallel_example.c $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

#############
# Utilities #
#############
utils: $(BUILD_UTIL_DIR)/rsa_key_generator $(BUILD_UTIL_DIR)/rsa_key_info $(BUILD_UTIL_DIR)/hash_optimal $(BUILD_UTIL_DIR)/lisp_syntax_highlighter $(BUILD_UTIL_DIR)/aes_assert $(BUILD_UTIL_DIR)/secure_socket_client $(BUILD_UTIL_DIR)/secure_socket_server 

$(BUILD_UTIL_DIR):
	@mkdir -p $@

$(BUILD_UTIL_DIR)/rsa_key_generator: $(UTILS_DIR)/rsa_key_generator.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/rsa_key_info: $(UTILS_DIR)/rsa_key_info.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/hash_optimal: $(UTILS_DIR)/hash_optimal.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/lisp_syntax_highlighter: $(UTILS_DIR)/lisp_syntax_highlighter.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/aes_assert: $(UTILS_DIR)/aes_assert.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/secure_socket_server: $(UTILS_DIR)/secure_socket_server.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/secure_socket_client: $(UTILS_DIR)/secure_socket_client.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

#########
# Tests #
#########
tests: $(BUILD_TEST_DIR)/pack_test $(BUILD_TEST_DIR)/hash_test $(BUILD_TEST_DIR)/array_test $(BUILD_TEST_DIR)/bitmap_test $(BUILD_TEST_DIR)/circular_buffer_test $(BUILD_TEST_DIR)/dict_test $(BUILD_TEST_DIR)/b_test $(BUILD_TEST_DIR)/binary_test $(BUILD_TEST_DIR)/tuple_test $(BUILD_TEST_DIR)/priority_queue_test $(BUILD_TEST_DIR)/queue_test $(BUILD_TEST_DIR)/stack_test $(BUILD_TEST_DIR)/base64_test $(BUILD_TEST_DIR)/json_test

$(BUILD_TEST_DIR):
	@mkdir -p $@

# core
$(BUILD_TEST_DIR)/pack_test: $(TESTS_DIR)/pack_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/hash_test: $(TESTS_DIR)/hash_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

# data
$(BUILD_TEST_DIR)/array_test: $(TESTS_DIR)/array_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/bitmap_test: $(TESTS_DIR)/bitmap_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/circular_buffer_test: $(TESTS_DIR)/circular_buffer_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/dict_test: $(TESTS_DIR)/dict_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/binary_test: $(TESTS_DIR)/binary_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/b_test: $(TESTS_DIR)/b_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/b.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/tuple_test: $(TESTS_DIR)/tuple_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/tuple.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/priority_queue_test: $(TESTS_DIR)/priority_queue_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/queue_test: $(TESTS_DIR)/queue_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/stack_test: $(TESTS_DIR)/stack_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/base64_test: $(TESTS_DIR)/base64_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/json_test: $(TESTS_DIR)/json_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ -lm $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

#########
# Clean #
#########
clean:
	rm -rf $(BUILD_LIB_DIR) $(BUILD_EXAMPLE_DIR) $(BUILD_TEST_DIR) $(BUILD_UTIL_DIR)
