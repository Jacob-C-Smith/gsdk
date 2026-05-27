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
	LDFLAGS += -lpthread -lm -pthread -O3
endif

# Compiler
CC = clang 
CFLAGS = -fPIC -Wall -Wextra -Iinclude -std=c23 -g

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
CORE_LIBS = interfaces log sync hash socket stream test pack 
CRYPTO_LIBS = rsa sha digital_signature aead ed25519 x25519 secure_socket
DATA_LIBS = array bitmap cache circular_buffer dict double_queue hash_table priority_queue queue set stack tuple avl binary red_black tree adjacency_list adjacency_matrix edge_list graph
REFLECTION_LIBS = base64 json
PERFORMANCE_LIBS = parallel

# Lists of targets
LIBS = $(CORE_LIBS) $(CRYPTO_LIBS) $(DATA_LIBS) $(REFLECTION_LIBS) $(PERFORMANCE_LIBS)
UTILS = rsa_key_generator rsa_key_info aes_assert sha256_hash digital_sign digital_verify echo_server certificate_chain_verify ed25519_key_generator certificate_create certificate_info certificate_sign certificate_verify echo_client time_server time_client hash_optimal lisp_syntax_highlighter

# Phony targets
.PHONY: all clean libs examples tests utils valgrind ed25519_test_vectors aead_test_vectors

#############
# Libraries #
#############
all: libs examples tests utils main

main: main.c $(addprefix $(BUILD_LIB_DIR)/, $(addsuffix .$(SHARED_EXT), $(LIBS)))
	# main
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o build/main main.c $(addprefix $(ROOT_DIR)/$(BUILD_LIB_DIR)/, $(addsuffix .$(SHARED_EXT), $(LIBS)))

libs: $(addprefix $(BUILD_LIB_DIR)/, $(addsuffix .$(SHARED_EXT), $(LIBS)))

$(BUILD_LIB_DIR):
	@mkdir -p $@

# Core
$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/interfaces/*.c) | $(BUILD_LIB_DIR)
	# lib > core > interfaces
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_LIB_DIR)/log.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/log/*.c) | $(BUILD_LIB_DIR)
	# lib > core > log
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/sync.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/sync/*.c) | $(BUILD_LIB_DIR)
	# lib > core > sync
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/hash.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/hash/*.c) | $(BUILD_LIB_DIR)
	# lib > core > hash
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/test.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/test/*.c) | $(BUILD_LIB_DIR)
	# lib > core > test
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/socket.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/socket/*.c) | $(BUILD_LIB_DIR)
	# lib > core > socket
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/stream.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/stream/*.c) | $(BUILD_LIB_DIR)
	# lib > core > stream
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/pack.$(SHARED_EXT): $(wildcard $(SRC_DIR)/core/pack/*.c) | $(BUILD_LIB_DIR)
	# lib > core > pack
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# Crypto
$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/rsa/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > rsa
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/aes.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/aes/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > aes
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/sha.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/sha/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > sha
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/digital_signature/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > digital signature
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/ed25519/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > ed25519
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/x25519.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/x25519/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > x25519
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/aead.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/aead/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > aead
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT): $(wildcard $(SRC_DIR)/crypto/secure_socket/*.c) | $(BUILD_LIB_DIR)
	# lib > crypto > secure socket
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT)

# Data structures
$(BUILD_LIB_DIR)/array.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/array/*.c) | $(BUILD_LIB_DIR)
	# lib > data > array
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/bitmap/*.c) | $(BUILD_LIB_DIR)
	# lib > data > bitmap
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/cache.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/cache/*.c) | $(BUILD_LIB_DIR)
	# lib > data > cache
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/circular_buffer/*.c) | $(BUILD_LIB_DIR)
	# lib > data > circular buffer
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/double_queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/double_queue/*.c) | $(BUILD_LIB_DIR)
	# lib > data > double queue
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/dict.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/dict/*.c) | $(BUILD_LIB_DIR)
	# lib > data > dict
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/red_black.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/red_black/*.c) | $(BUILD_LIB_DIR)
	# lib > data > tree > red black
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/avl.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/avl/*.c) | $(BUILD_LIB_DIR)
	# lib > data > tree > avl
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/binary.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/binary/*.c) | $(BUILD_LIB_DIR)
	# lib > data > tree > binary
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/tree.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/tree/*.c) | $(BUILD_LIB_DIR)
	# lib > data > tree
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/avl.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/red_black.$(SHARED_EXT)

$(BUILD_LIB_DIR)/tuple.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/tuple/*.c) | $(BUILD_LIB_DIR)
	# lib > data > tuple
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/priority_queue/*.c) | $(BUILD_LIB_DIR)
	# lib > data > priority queue
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/queue.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/queue/*.c) | $(BUILD_LIB_DIR)
	# lib > data > queue
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/set.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/set/*.c) | $(BUILD_LIB_DIR)
	# lib > data > set
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/stack.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/stack/*.c) | $(BUILD_LIB_DIR)
	# lib > data > stack
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/hash_table.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/hash_table/*.c) | $(BUILD_LIB_DIR)
	# lib > data > hash table
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/adjacency_list.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/adjacency_list/*.c) | $(BUILD_LIB_DIR)
	# lib > data > graph > adjacency list
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/adjacency_matrix.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/adjacency_matrix/*.c) | $(BUILD_LIB_DIR)
	# lib > data > graph > adjacency matrix
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/edge_list.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/edge_list/*.c) | $(BUILD_LIB_DIR)
	# lib > data > graph > edge list
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/graph.$(SHARED_EXT): $(wildcard $(SRC_DIR)/data/graph/*.c) | $(BUILD_LIB_DIR)
	# lib > data > graph
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash_table.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/adjacency_matrix.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/adjacency_list.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/edge_list.$(SHARED_EXT) 

# Reflection
$(BUILD_LIB_DIR)/base64.$(SHARED_EXT): $(wildcard $(SRC_DIR)/reflection/base64/*.c) | $(BUILD_LIB_DIR)
	# lib > reflection > base64
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_LIB_DIR)/json.$(SHARED_EXT): $(wildcard $(SRC_DIR)/reflection/json/*.c) | $(BUILD_LIB_DIR)
	# lib > reflection > json
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# Performance
$(BUILD_LIB_DIR)/parallel.$(SHARED_EXT): $(wildcard $(SRC_DIR)/performance/parallel/*.c) | $(BUILD_LIB_DIR)
	# lib > performance > parallel
	@$(CC) $(CFLAGS) $(SHARED_FLAGS) $(RPATH_FLAGS) $(LDFLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

############
# Examples #
############
examples:  $(BUILD_EXAMPLE_DIR)/interfaces_example $(BUILD_EXAMPLE_DIR)/log_example $(BUILD_EXAMPLE_DIR)/sync_example $(BUILD_EXAMPLE_DIR)/hash_example $(BUILD_EXAMPLE_DIR)/stream_example $(BUILD_EXAMPLE_DIR)/pack_example $(BUILD_EXAMPLE_DIR)/rsa_example $(BUILD_EXAMPLE_DIR)/sha_example $(BUILD_EXAMPLE_DIR)/digital_signature_example $(BUILD_EXAMPLE_DIR)/chacha20_example $(BUILD_EXAMPLE_DIR)/poly1305_example $(BUILD_EXAMPLE_DIR)/aead_example $(BUILD_EXAMPLE_DIR)/ed25519_example $(BUILD_EXAMPLE_DIR)/x25519_example $(BUILD_EXAMPLE_DIR)/array_example $(BUILD_EXAMPLE_DIR)/bitmap_example $(BUILD_EXAMPLE_DIR)/cache_example $(BUILD_EXAMPLE_DIR)/circular_buffer_example $(BUILD_EXAMPLE_DIR)/dict_example $(BUILD_EXAMPLE_DIR)/double_queue_example $(BUILD_EXAMPLE_DIR)/hash_table_example $(BUILD_EXAMPLE_DIR)/priority_queue_example $(BUILD_EXAMPLE_DIR)/queue_example $(BUILD_EXAMPLE_DIR)/set_example $(BUILD_EXAMPLE_DIR)/stack_example $(BUILD_EXAMPLE_DIR)/tuple_example $(BUILD_EXAMPLE_DIR)/avl_example $(BUILD_EXAMPLE_DIR)/binary_example $(BUILD_EXAMPLE_DIR)/red_black_example $(BUILD_EXAMPLE_DIR)/tree_example $(BUILD_EXAMPLE_DIR)/graph_example $(BUILD_EXAMPLE_DIR)/base64_example $(BUILD_EXAMPLE_DIR)/json_example $(BUILD_EXAMPLE_DIR)/parallel_example 
$(BUILD_EXAMPLE_DIR):
	@mkdir -p $@

$(BUILD_EXAMPLE_DIR)/log_example: $(EXAMPLES_DIR)/log_example.c $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > core > log
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/sync_example: $(EXAMPLES_DIR)/sync_example.c $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > core > sync
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/pack_example: $(EXAMPLES_DIR)/pack_example.c $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > core > pack
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/stream_example: $(EXAMPLES_DIR)/stream_example.c $(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > core > stream
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/interfaces_example: $(EXAMPLES_DIR)/interfaces_example.c $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > core > interfaces
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/hash_example: $(EXAMPLES_DIR)/hash_example.c $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > core > hash
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/rsa_example: $(EXAMPLES_DIR)/rsa_example.c $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > rsa
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/digital_signature_example: $(EXAMPLES_DIR)/digital_signature_example.c $(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT) $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > digital signature
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/sha_example: $(EXAMPLES_DIR)/sha_example.c $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > sha
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/ed25519_example: $(EXAMPLES_DIR)/ed25519_example.c $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > ed25519
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/x25519_example: $(EXAMPLES_DIR)/x25519_example.c $(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > x25519
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/chacha20_example: $(EXAMPLES_DIR)/chacha20_example.c $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > aead > chacha20
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/poly1305_example: $(EXAMPLES_DIR)/poly1305_example.c $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > aead > poly1305
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/aead_example: $(EXAMPLES_DIR)/aead_example.c $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > crypto > aead
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/array_example: $(EXAMPLES_DIR)/array_example.c $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > array
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/bitmap_example: $(EXAMPLES_DIR)/bitmap_example.c $(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > bitmap
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/cache_example: $(EXAMPLES_DIR)/cache_example.c $(BUILD_LIB_DIR)/cache.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > cache
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/circular_buffer_example: $(EXAMPLES_DIR)/circular_buffer_example.c $(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > circular buffer
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/dict_example: $(EXAMPLES_DIR)/dict_example.c $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > dict
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/double_queue_example: $(EXAMPLES_DIR)/double_queue_example.c $(BUILD_LIB_DIR)/double_queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > double queue
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/avl_example: $(EXAMPLES_DIR)/avl_example.c $(BUILD_LIB_DIR)/avl.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > avl
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/red_black_example: $(EXAMPLES_DIR)/red_black_example.c $(BUILD_LIB_DIR)/red_black.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > red black
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/binary_example: $(EXAMPLES_DIR)/binary_example.c $(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > binary
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/tree_example: $(EXAMPLES_DIR)/tree_example.c $(BUILD_LIB_DIR)/tree.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/avl.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/red_black.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > tree
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/graph_example: $(EXAMPLES_DIR)/graph_example.c $(BUILD_LIB_DIR)/graph.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/adjacency_matrix.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/adjacency_list.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/edge_list.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > graph
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/tuple_example: $(EXAMPLES_DIR)/tuple_example.c $(BUILD_LIB_DIR)/tuple.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT)  $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > tuple
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/priority_queue_example: $(EXAMPLES_DIR)/priority_queue_example.c $(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > priority queue
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/queue_example: $(EXAMPLES_DIR)/queue_example.c $(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > queue
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/set_example: $(EXAMPLES_DIR)/set_example.c $(BUILD_LIB_DIR)/set.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > set
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/stack_example: $(EXAMPLES_DIR)/stack_example.c $(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > stack
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/hash_table_example: $(EXAMPLES_DIR)/hash_table_example.c $(BUILD_LIB_DIR)/hash_table.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > data > hash table
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/base64_example: $(EXAMPLES_DIR)/base64_example.c $(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > reflection > base64
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

$(BUILD_EXAMPLE_DIR)/json_example: $(EXAMPLES_DIR)/json_example.c $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > reflection > json
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ 

$(BUILD_EXAMPLE_DIR)/parallel_example: $(EXAMPLES_DIR)/parallel_example.c $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) | $(BUILD_EXAMPLE_DIR)
	# example > performance > parallel
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^

#############
# Utilities #
#############
utils: $(BUILD_UTIL_DIR)/certificate_create $(BUILD_UTIL_DIR)/certificate_info $(BUILD_UTIL_DIR)/certificate_sign $(BUILD_UTIL_DIR)/certificate_verify $(BUILD_UTIL_DIR)/certificate_chain_verify  $(BUILD_UTIL_DIR)/rsa_key_generator $(BUILD_UTIL_DIR)/rsa_key_info $(BUILD_UTIL_DIR)/sha256_hash $(BUILD_UTIL_DIR)/sha512_hash $(BUILD_UTIL_DIR)/digital_sign $(BUILD_UTIL_DIR)/digital_verify $(BUILD_UTIL_DIR)/ed25519_key_generator $(BUILD_UTIL_DIR)/hash_optimal $(BUILD_UTIL_DIR)/lisp_syntax_highlighter $(BUILD_UTIL_DIR)/echo_server $(BUILD_UTIL_DIR)/echo_client $(BUILD_UTIL_DIR)/time_server $(BUILD_UTIL_DIR)/time_client $(BUILD_UTIL_DIR)/secure_time_server $(BUILD_UTIL_DIR)/secure_time_client $(BUILD_UTIL_DIR)/secure_echo_server $(BUILD_UTIL_DIR)/secure_echo_client

$(BUILD_UTIL_DIR):
	@mkdir -p $@

$(BUILD_UTIL_DIR)/rsa_key_generator: $(UTILS_DIR)/crypto/rsa_key_generator.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > rsa key generator
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/rsa_key_info: $(UTILS_DIR)/crypto/rsa_key_info.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > rsa key info
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/hash_optimal: $(UTILS_DIR)/misc/hash_optimal.c | $(BUILD_UTIL_DIR)
	# utilities > misc > hash optimal
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/lisp_syntax_highlighter: $(UTILS_DIR)/misc/lisp_syntax_highlighter.c | $(BUILD_UTIL_DIR)
	# utilities > misc > lisp syntax highlighter
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/sha256_hash: $(UTILS_DIR)/crypto/sha256_hash.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > sha256 hash
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/sha512_hash: $(UTILS_DIR)/crypto/sha512_hash.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > sha512 hash
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/digital_sign: $(UTILS_DIR)/crypto/digital_sign.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > digital sign
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/digital_verify: $(UTILS_DIR)/crypto/digital_verify.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > digital verify
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/digital_signature.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/ed25519_key_generator: $(UTILS_DIR)/crypto/ed25519_key_generator.c | $(BUILD_UTIL_DIR)
	# utilities > crypto > ed25519 key generator
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/certificate_create: $(UTILS_DIR)/certificate/certificate_create.c | $(BUILD_UTIL_DIR)
	# utilities > certificate > create
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/certificate_info: $(UTILS_DIR)/certificate/certificate_info.c | $(BUILD_UTIL_DIR)
	# utilities > certificate > info
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/certificate_sign: $(UTILS_DIR)/certificate/certificate_sign.c | $(BUILD_UTIL_DIR)
	# utilities > certificate > sign
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/certificate_verify: $(UTILS_DIR)/certificate/certificate_verify.c | $(BUILD_UTIL_DIR)
	# utilities > certificate > verify
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/certificate_chain_verify: $(UTILS_DIR)/certificate/certificate_chain_verify.c | $(BUILD_UTIL_DIR)
	# utilities > certificate > chain verify
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_UTIL_DIR)/echo_server: $(UTILS_DIR)/network/udp/echo_server.c | $(BUILD_UTIL_DIR)
	# utilities > network > udp > echo server
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/echo_client: $(UTILS_DIR)/network/udp/echo_client.c | $(BUILD_UTIL_DIR)
	# utilities > network > udp > echo client
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/rsa.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/time_server: $(UTILS_DIR)/network/tcp/time_server.c | $(BUILD_UTIL_DIR)
	# utilities > network > tcp > time server
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/time_client: $(UTILS_DIR)/network/tcp/time_client.c | $(BUILD_UTIL_DIR)
	# utilities > network > tcp > time client
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/secure_time_server: $(UTILS_DIR)/network/secure/secure_time_server.c | $(BUILD_UTIL_DIR)
	# utilities > network > secure > secure time server
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/secure_time_client: $(UTILS_DIR)/network/secure/secure_time_client.c | $(BUILD_UTIL_DIR)
	# utilities > network > secure > secure time client
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/secure_echo_server: $(UTILS_DIR)/network/secure/secure_echo_server.c | $(BUILD_UTIL_DIR)
	# utilities > network > secure > secure echo server
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_UTIL_DIR)/secure_echo_client: $(UTILS_DIR)/network/secure/secure_echo_client.c | $(BUILD_UTIL_DIR)
	# utilities > network > secure > secure echo client
	@$(CC) $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/secure_socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(BUILD_LIB_DIR)/socket.$(SHARED_EXT) $(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

#########
# Tests #
#########
tests: aead_test_vectors ed25519_test_vectors x25519_test_vectors $(BUILD_TEST_DIR)/sync_test $(BUILD_TEST_DIR)/stream_test $(BUILD_TEST_DIR)/pack_test $(BUILD_TEST_DIR)/hash_test $(BUILD_TEST_DIR)/sha_test $(BUILD_TEST_DIR)/aead_test $(BUILD_TEST_DIR)/ed25519_test $(BUILD_TEST_DIR)/x25519_test $(BUILD_TEST_DIR)/array_test $(BUILD_TEST_DIR)/bitmap_test $(BUILD_TEST_DIR)/cache_test $(BUILD_TEST_DIR)/circular_buffer_test $(BUILD_TEST_DIR)/dict_test $(BUILD_TEST_DIR)/double_queue_test $(BUILD_TEST_DIR)/hash_table_test $(BUILD_TEST_DIR)/tree_test $(BUILD_TEST_DIR)/tuple_test $(BUILD_TEST_DIR)/priority_queue_test $(BUILD_TEST_DIR)/queue_test $(BUILD_TEST_DIR)/set_test $(BUILD_TEST_DIR)/stack_test $(BUILD_TEST_DIR)/base64_test $(BUILD_TEST_DIR)/json_test $(BUILD_TEST_DIR)/thread_pool_test $(BUILD_TEST_DIR)/schedule_test

$(BUILD_TEST_DIR):
	@mkdir -p $@

# core
$(BUILD_TEST_DIR)/sync_test: $(TESTS_DIR)/sync_test.c | $(BUILD_TEST_DIR)
	# test > core > sync
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT)  

$(BUILD_TEST_DIR)/stream_test: $(TESTS_DIR)/stream_test.c | $(BUILD_TEST_DIR)
	# test > core > stream
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/test.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/pack_test: $(TESTS_DIR)/pack_test.c | $(BUILD_TEST_DIR)
	# test > core > pack
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/test.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/hash_test: $(TESTS_DIR)/hash_test.c | $(BUILD_TEST_DIR)
	# test > core > hash
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

# crypto
$(BUILD_TEST_DIR)/aead_test: $(TESTS_DIR)/aead_test.c | $(BUILD_TEST_DIR)
	# test > crypto > aead
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/aead.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/ed25519_test: $(TESTS_DIR)/ed25519_test.c | $(BUILD_TEST_DIR)
	# test > crypto > ed25519
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(BUILD_LIB_DIR)/ed25519.$(SHARED_EXT) $(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/sha_test: $(TESTS_DIR)/sha_test.c | $(BUILD_TEST_DIR)
	# test > crypto > sha
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sha.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

$(BUILD_TEST_DIR)/x25519_test: $(TESTS_DIR)/x25519_test.c | $(BUILD_TEST_DIR)
	# test > crypto > x25519
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/x25519.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT) 

# data
$(BUILD_TEST_DIR)/array_test: $(TESTS_DIR)/array_test.c | $(BUILD_TEST_DIR)
	# test > data > array
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/bitmap_test: $(TESTS_DIR)/bitmap_test.c | $(BUILD_TEST_DIR)
	# test > data > bitmap
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/bitmap.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/cache_test: $(TESTS_DIR)/cache_test.c | $(BUILD_TEST_DIR)
	# test > data > cache
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/cache.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/circular_buffer_test: $(TESTS_DIR)/circular_buffer_test.c | $(BUILD_TEST_DIR)
	# test > data > circular buffer
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/circular_buffer.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/double_queue_test: $(TESTS_DIR)/double_queue_test.c | $(BUILD_TEST_DIR)
	# test > data > double queue
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/double_queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/hash_table_test: $(TESTS_DIR)/hash_table_test.c | $(BUILD_TEST_DIR)
	# test > data > hash table
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash_table.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/test.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/dict_test: $(TESTS_DIR)/dict_test.c | $(BUILD_TEST_DIR)
	# test > data > dict
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/tree_test: $(TESTS_DIR)/tree_test.c | $(BUILD_TEST_DIR)
	# test > data > tree
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/tree.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/binary.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/avl.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/red_black.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/tuple_test: $(TESTS_DIR)/tuple_test.c | $(BUILD_TEST_DIR)
	# test > data > tuple
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/tuple.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/priority_queue_test: $(TESTS_DIR)/priority_queue_test.c | $(BUILD_TEST_DIR)
	# test > data > priority queue
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/priority_queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT)  $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/queue_test: $(TESTS_DIR)/queue_test.c | $(BUILD_TEST_DIR)
	# test > data > queue
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/queue.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/set_test: $(TESTS_DIR)/set_test.c | $(BUILD_TEST_DIR)
	# test > data > set
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/set.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/stack_test: $(TESTS_DIR)/stack_test.c | $(BUILD_TEST_DIR)
	# test > data > stack
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/stack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# reflection
$(BUILD_TEST_DIR)/base64_test: $(TESTS_DIR)/base64_test.c | $(BUILD_TEST_DIR)
	# test > reflection > base64
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/base64.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/json_test: $(TESTS_DIR)/json_test.c | $(BUILD_TEST_DIR)
	# test > reflection > json
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/json.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/test.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/hash.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/array.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/dict.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

# performance
$(BUILD_TEST_DIR)/thread_pool_test: $(TESTS_DIR)/thread_pool_test.c | $(BUILD_TEST_DIR)
	# test > performance > thread pool
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/test.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

$(BUILD_TEST_DIR)/schedule_test: $(TESTS_DIR)/schedule_test.c | $(BUILD_TEST_DIR)
	# test > performance > schedule
	@$(CC) $(CFLAGS) $(RPATH_FLAGS) -o $@ $^ $(ROOT_DIR)/$(BUILD_LIB_DIR)/parallel.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/log.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/stream.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/pack.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/sync.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/test.$(SHARED_EXT) $(ROOT_DIR)/$(BUILD_LIB_DIR)/interfaces.$(SHARED_EXT)

############
# Valgrind #
############
valgrind: libs examples
	@mkdir -p output/valgrind output/valgrind/core output/valgrind/data output/valgrind/reflection output/valgrind/performance
	@./scripts/packages.sh _example output/valgrind

################
# Test vectors #
################
aead_test_vectors:
	@python3 scripts/aead-test-vectors.py > src/crypto/aead/aead_test.h

ed25519_test_vectors:
	@awk -f scripts/ed25519-test-vectors.awk scripts/sign.input > src/crypto/ed25519/ed25519_test.h

x25519_test_vectors:
	@python3 scripts/x25519-test-vectors.py > src/crypto/x25519/x25519_test.h

#########
# Clean #
#########
clean:
	rm -rf $(BUILD_LIB_DIR) $(BUILD_EXAMPLE_DIR) $(BUILD_TEST_DIR) $(BUILD_UTIL_DIR) $(VALGRIND_LOGS_DIR)