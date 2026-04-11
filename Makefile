# Cross-platform Makefile for GSDK

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	SHARED_EXT = dylib
	SHARED_FLAGS = -dynamiclib -install_name @rpath/$(@F)
	RPATH_FLAGS = -Wl,-rpath,@executable_path/../lib
	LDFLAGS += -lpthread -lm -O3
else
	SHARED_EXT = so
	SHARED_FLAGS = -shared -Wl,-soname,$(@F)
	RPATH_FLAGS = -Wl,-rpath,'$$ORIGIN/../lib'
	LDFLAGS += -lpthread -pthread -O3
endif

# Compiler
CC = clang #/opt/gcc-15.1.0/bin/gcc
CFLAGS = -fsanitize=address -fno-omit-frame-pointer -fPIC -Wall -Wextra -Iinclude -Iinclude/core -Iinclude/data -Iinclude/performance -Iinclude/reflection -std=c23 -g

# Directories
SRC_DIR           = src
BUILD_DIR         = build
BUILD_LIB_DIR     = $(BUILD_DIR)/lib
BUILD_EXAMPLE_DIR = $(BUILD_DIR)/examples
BUILD_TEST_DIR    = $(BUILD_DIR)/tests
BUILD_UTIL_DIR    = $(BUILD_DIR)/utilities
VALGRIND_LOGS_DIR = output/valgrind
EXAMPLES_DIR      = $(SRC_DIR)/examples
TESTS_DIR         = $(SRC_DIR)/test
UTILS_DIR         = $(SRC_DIR)/utilities
ROOT_DIR          = $(shell pwd)

# Core libraries
CORE_LIBS = interfaces log sync pack rsa sha digital_signature hash socket ed25519 aead
DATA_LIBS = array bitmap cache circular_buffer dict double_queue binary tuple priority_queue queue set stack hash_table
REFLECTION_LIBS = base64 json
PERFORMANCE_LIBS = parallel

# Lists of targets
LIBS = $(CORE_LIBS) $(DATA_LIBS) $(REFLECTION_LIBS) $(PERFORMANCE_LIBS)
TESTS = $(DATA_LIBS) $(REFLECTION_LIBS)
UTILS = rsa_key_generator rsa_key_info hash_optimal lisp_syntax_highlighter aes_assert sha256_hash digital_sign digital_verify echo_server

# Phony targets
.PHONY: all clean libs examples utils tests valgrind ed25519_test_vectors

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

$(BUILD_LIB_DIR)/aes.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/aes/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/sha.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/sha/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/digital_signature/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/hash.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/hash/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/socket.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/socket/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/interfaces/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/ed25519/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/aead.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/aead/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# Data structures
$(BUILD_LIB_DIR)/array.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/array/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/bitmap/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/cache.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/cache/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/circular_buffer/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/double_queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/double_queue/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/dict.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/dict/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/binary.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/binary/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/tuple.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/tuple/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/priority_queue/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/queue/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/set.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/set/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/stack.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/stack/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/hash_table.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/hash_table/*.c) | $(BUILD_LIB_DIR)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

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
examples: $(BUILD_EXAMPLE_DIR)/array_example $(BUILD_EXAMPLE_DIR)/base64_example $(BUILD_EXAMPLE_DIR)/binary_example $(BUILD_EXAMPLE_DIR)/bitmap_example $(BUILD_EXAMPLE_DIR)/cache_example $(BUILD_EXAMPLE_DIR)/chacha20_example $(BUILD_EXAMPLE_DIR)/circular_buffer_example $(BUILD_EXAMPLE_DIR)/dict_example $(BUILD_EXAMPLE_DIR)/digital_signature_example $(BUILD_EXAMPLE_DIR)/double_queue_example $(BUILD_EXAMPLE_DIR)/ed25519_example $(BUILD_EXAMPLE_DIR)/hash_example $(BUILD_EXAMPLE_DIR)/hash_table_example $(BUILD_EXAMPLE_DIR)/interfaces_example $(BUILD_EXAMPLE_DIR)/json_example $(BUILD_EXAMPLE_DIR)/log_example $(BUILD_EXAMPLE_DIR)/pack_example $(BUILD_EXAMPLE_DIR)/parallel_example $(BUILD_EXAMPLE_DIR)/priority_queue_example $(BUILD_EXAMPLE_DIR)/queue_example $(BUILD_EXAMPLE_DIR)/rsa_example $(BUILD_EXAMPLE_DIR)/set_example $(BUILD_EXAMPLE_DIR)/sha_example $(BUILD_EXAMPLE_DIR)/socket_example $(BUILD_EXAMPLE_DIR)/stack_example $(BUILD_EXAMPLE_DIR)/sync_example $(BUILD_EXAMPLE_DIR)/tuple_example
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

$(BUILD_EXAMPLE_DIR)/digital_signature_example: $(EXAMPLES_DIR)/digital_signature_example.c $(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT) $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/sha_example: $(EXAMPLES_DIR)/sha_example.c $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/interfaces_example: $(EXAMPLES_DIR)/interfaces_example.c $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/hash_example: $(EXAMPLES_DIR)/hash_example.c $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/socket_example: $(EXAMPLES_DIR)/socket_example.c $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/ed25519_example: $(EXAMPLES_DIR)/ed25519_example.c $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/chacha20_example: $(EXAMPLES_DIR)/chacha20_example.c $(BUILD_LIB_DIR)/chacha20.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/array_example: $(EXAMPLES_DIR)/array_example.c $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/bitmap_example: $(EXAMPLES_DIR)/bitmap_example.c $(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/cache_example: $(EXAMPLES_DIR)/cache_example.c $(BUILD_LIB_DIR)/cache.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/circular_buffer_example: $(EXAMPLES_DIR)/circular_buffer_example.c $(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/dict_example: $(EXAMPLES_DIR)/dict_example.c $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/double_queue_example: $(EXAMPLES_DIR)/double_queue_example.c $(BUILD_LIB_DIR)/double_queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/binary_example: $(EXAMPLES_DIR)/binary_example.c $(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/tuple_example: $(EXAMPLES_DIR)/tuple_example.c $(BUILD_LIB_DIR)/tuple.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT)  $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/priority_queue_example: $(EXAMPLES_DIR)/priority_queue_example.c $(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/queue_example: $(EXAMPLES_DIR)/queue_example.c $(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/set_example: $(EXAMPLES_DIR)/set_example.c $(BUILD_LIB_DIR)/set.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/stack_example: $(EXAMPLES_DIR)/stack_example.c $(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/hash_table_example: $(EXAMPLES_DIR)/hash_table_example.c $(BUILD_LIB_DIR)/hash_table.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/base64_example: $(EXAMPLES_DIR)/base64_example.c $(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/json_example: $(EXAMPLES_DIR)/json_example.c $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ -lm

$(BUILD_EXAMPLE_DIR)/parallel_example: $(EXAMPLES_DIR)/parallel_example.c $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

#############
# Utilities #
#############
utils: $(BUILD_UTIL_DIR)/rsa_key_generator $(BUILD_UTIL_DIR)/rsa_key_info $(BUILD_UTIL_DIR)/hash_optimal $(BUILD_UTIL_DIR)/lisp_syntax_highlighter $(BUILD_UTIL_DIR)/sha256_hash $(BUILD_UTIL_DIR)/sha512_hash $(BUILD_UTIL_DIR)/digital_sign $(BUILD_UTIL_DIR)/digital_verify $(BUILD_UTIL_DIR)/echo_server

$(BUILD_UTIL_DIR):
	@mkdir -p $@

$(BUILD_UTIL_DIR)/rsa_key_generator: $(UTILS_DIR)/rsa_key_generator.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/rsa_key_info: $(UTILS_DIR)/rsa_key_info.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/hash_optimal: $(UTILS_DIR)/hash_optimal.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/lisp_syntax_highlighter: $(UTILS_DIR)/lisp_syntax_highlighter.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/sha256_hash: $(UTILS_DIR)/sha256_hash.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/sha512_hash: $(UTILS_DIR)/sha512_hash.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/digital_sign: $(UTILS_DIR)/digital_sign.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/digital_verify: $(UTILS_DIR)/digital_verify.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/echo_server: $(UTILS_DIR)/echo_server.c | $(BUILD_UTIL_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

#########
# Tests #
#########
tests: $(BUILD_TEST_DIR)/sync_test $(BUILD_TEST_DIR)/pack_test $(BUILD_TEST_DIR)/hash_test $(BUILD_TEST_DIR)/sha_test $(BUILD_TEST_DIR)/ed25519_test $(BUILD_TEST_DIR)/array_test $(BUILD_TEST_DIR)/bitmap_test $(BUILD_TEST_DIR)/cache_test $(BUILD_TEST_DIR)/circular_buffer_test $(BUILD_TEST_DIR)/dict_test $(BUILD_TEST_DIR)/double_queue_test $(BUILD_TEST_DIR)/binary_test $(BUILD_TEST_DIR)/tuple_test $(BUILD_TEST_DIR)/priority_queue_test $(BUILD_TEST_DIR)/queue_test $(BUILD_TEST_DIR)/set_test $(BUILD_TEST_DIR)/stack_test $(BUILD_TEST_DIR)/base64_test $(BUILD_TEST_DIR)/json_test

$(BUILD_TEST_DIR):
	@mkdir -p $@

# core
$(BUILD_TEST_DIR)/sync_test: $(TESTS_DIR)/sync_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT)  

$(BUILD_TEST_DIR)/pack_test: $(TESTS_DIR)/pack_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/hash_test: $(TESTS_DIR)/hash_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/sha_test: $(TESTS_DIR)/sha_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/ed25519_test: $(TESTS_DIR)/ed25519_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) 

# data
$(BUILD_TEST_DIR)/array_test: $(TESTS_DIR)/array_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/bitmap_test: $(TESTS_DIR)/bitmap_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/cache_test: $(TESTS_DIR)/cache_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/cache.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/circular_buffer_test: $(TESTS_DIR)/circular_buffer_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/double_queue_test: $(TESTS_DIR)/double_queue_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/double_queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/dict_test: $(TESTS_DIR)/dict_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/binary_test: $(TESTS_DIR)/binary_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/tuple_test: $(TESTS_DIR)/tuple_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/tuple.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/priority_queue_test: $(TESTS_DIR)/priority_queue_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/queue_test: $(TESTS_DIR)/queue_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/set_test: $(TESTS_DIR)/set_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/set.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/stack_test: $(TESTS_DIR)/stack_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# reflection
$(BUILD_TEST_DIR)/base64_test: $(TESTS_DIR)/base64_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/json_test: $(TESTS_DIR)/json_test.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ -lm $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

############
# Valgrind #
############
valgrind: libs examples
	@mkdir -p output/valgrind output/valgrind/core output/valgrind/data output/valgrind/reflection output/valgrind/performance
	@./scripts/packages.sh _example output/valgrind

########################
# Ed25519 test vectors #
########################
ed25519_test_vectors:
	@awk -f scripts/ed25519-test-vectors.awk scripts/sign.input > src/core/ed25519/ed25519_test.h

#########
# Clean #
#########
clean:
	rm -rf $(BUILD_LIB_DIR) $(BUILD_EXAMPLE_DIR) $(BUILD_TEST_DIR) $(BUILD_UTIL_DIR) $(VALGRIND_LOGS_DIR)