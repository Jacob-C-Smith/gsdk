 Preparing for a 1.0 release is a huge milestone, especially for a foundational C SDK like gsdk. To ensure it's
  "extremely solid," you'll want to verify memory safety, thread safety, cryptographic correctness, test coverage, and
  API stability.

  Here is a comprehensive 1.0 release checklist organized by module and project-wide infrastructure.
  1. 🏗️ Core Module (src/core/)
  The foundation of the SDK. Any bugs here cascade throughout the entire project.
   - [ ] Memory Allocator (interfaces): Verify that default_allocator behaves predictably in out-of-memory (OOM)
     conditions (returns NULL and doesn't crash). Ensure all allocations across the SDK properly check for this NULL
     return value.
   - [ ] Hashing (hash): Ensure uniform distribution and collision resistance for hash_fnv64, hash_mmh64, hash_xxh64,
     and hash_crc64 with edge cases (e.g., zero-length inputs, highly repetitive data).
   - [ ] Logging (log): Verify thread safety for logging if multiple threads attempt to write to log_file
     concurrently. Consider adding a lock to log.c operations if not already present.
   - [ ] Binary Serialization (pack): Validate endianness handling across different architectures. Ensure buffer
     overflow protections are in place when unpacking data.
   - [ ] Networking (socket, tcp, udp): Test cross-platform socket behavior (POSIX vs. Winsock). Ensure graceful
     teardown and socket closure on timeout, disconnection, or SIGPIPE.
   - [ ] Streams (stream): Validate edge cases in stream copying/piping (stream_copy, stream_pipe) and ensure EOF is
     accurately reported.
   - [ ] Testing Framework (test): Ensure the test runner accurately reports isolated failures without crashing the
     entire suite.

  2. 🔐 Crypto Module (src/crypto/)
  Cryptographic primitives require extreme scrutiny. 
   - [ ] AEAD / ChaCha20 / Poly1305: Validate against official RFC test vectors (e.g., RFC 8439). Verify nonce reuse
     protections or warnings.
   - [ ] Elliptic Curves (ed25519, x25519): Execute Wycheproof test vectors (I see you have
     scripts/vectors/wycheproof/) to ensure resistance against invalid curve attacks and edge cases.
   - [ ] Hashing (sha): Validate SHA-256 and SHA-512 against NIST test vectors.
   - [ ] Secure Sockets & Certificates: 
     - Ensure handshake state machines cannot be stalled or manipulated by malformed client/server hellos.
     - Test certificate chain validation (certificate_chain_verify) with expired, self-signed, and revoked certs.
     - Run fuzzing against the packet decryption routines.

  3. 💾 Data Structures (src/data/)
  Memory leaks and memory corruption often hide in collection implementations.
   - [ ] Memory Leaks: Run the entire test suite through Valgrind (via scripts/memcheck.sh) and assert 0 bytes lost.
     Pay special attention to complex teardowns like graph_destroy and tree_destroy.
   - [ ] Bounds Checking: Ensure collections like array, circular_buffer, stack, and queue handle capacity limits
     safely and reject out-of-bounds indexing gracefully.
   - [ ] Graphs (adjacency_list, adjacency_matrix, edge_list): Test memory limits on dense vs. sparse graphs. Verify
     that graph_algorithm_sssp_dijkstra and graph_algorithm_apsp_floyd_warshall handle unreachable nodes/disconnected
     components correctly.
   - [ ] Trees (avl, binary, red_black): Exhaustively test left/right rotation logic. A single missed pointer
     assignment here causes silent corruption.
   - [ ] Hash Tables (dict, hash_table, set): Verify resize/rehashing logic triggers correctly and preserves all data
     when the load factor gets too high. Ensure tombstone handling during removal does not break subsequent searches.
   - [ ] Thread Safety: Ensure the internal mutex locking pattern is strictly obeyed during all reads and writes
     across all collections.

  4. ⚡ Performance Module (src/performance/)
  Concurrency bugs are notoriously hard to reproduce.
   - [ ] Thread Pool (thread_pool): Test under heavy contention (more tasks than threads). Ensure thread_pool_destroy
     correctly signals and joins all worker threads without deadlocking or leaving zombie threads.
   - [ ] Task Scheduling (schedule, parallel): Verify dependency resolution (wait_quantity logic) correctly resolves
     DAGs (Directed Acyclic Graphs) of tasks and catches/rejects circular dependencies.
   - [ ] Race Conditions: Run the test suite with ThreadSanitizer (-fsanitize=thread) to catch data races.

  5. 🪞 Reflection / Parsing (src/reflection/)
  Parsers are prime targets for fuzzing and security exploits.
   - [ ] JSON Parser (json): 
     - Fuzz the JSON parser with deeply nested objects, massive strings, and malformed characters.
     - Ensure json_destroy/json_allocator correctly cleans up the entire nested DOM tree without stack overflow on
       deep recursion.
     - Verify strict adherence to precision limits for floats (JSON_VALUE_NUMBER).
   - [ ] Base64 (base64): Ensure padding (=, ==) is handled correctly and that malformed or non-alphabet characters
     are safely rejected during decoding.

  6. 🌐 Project-Wide Infrastructure
   - [ ] Static Analysis: Run Clang-Tidy or Cppcheck across the entire codebase to catch uninitialized variables,
     unused functions, and implicit cast issues.
   - [ ] Header Independence: Ensure every .h file can be included on its own without requiring prerequisites (they
     should all have #pragma once and necessary #includes).
   - [ ] Compilation Matrix: Verify the project builds completely warning-free (-Wall -Wextra -Werror -pedantic) on:
     - GCC (Linux/macOS)
     - Clang (Linux/macOS)
     - MSVC / MinGW (Windows) - I noticed some #ifdef _WIN64 stubs that need verification.
   - [ ] API Stability: Review all exposed functions in the headers. Ensure the naming (snake_case), pointer
     signatures, and 1/0 return paradigms documented in GEMINI.md are 100% consistent before freezing the 1.0 API.
   - [ ] Documentation: Ensure all Doxygen /** ! ... */ blocks are accurate, and generate the final API documentation
     to verify there are no missing parameters.

  Would you like me to dive into a specific module and begin executing any of these verification steps (e.g., running
  the test suite, looking for uninitialized variables, or setting up a fuzzer for the JSON parser)?