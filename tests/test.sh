#!/bin/bash

# This script runs all executable tests found in the build/tests directory
# and saves their output to log files.

# Directory where test executables are located
TEST_EXEC_DIR="../build/tests/"

# Directory to store test logs
LOG_DIR="output"

# Ensure the log directory exists
mkdir -p "$LOG_DIR"

# Find all executable files in the test directory.
# Using glob pattern to find all files, then filter for executables.
shopt -s nullglob  # Handle case where no files match
test_executables=()
for file in "$TEST_EXEC_DIR"/*; do
  if [[ -f "$file" && -x "$file" ]]; then
    test_executables+=("$file")
  fi
done
shopt -u nullglob  # Reset nullglob

# Check if any tests were found
if [ ${#test_executables[@]} -eq 0 ]; then
  echo "No executable tests found in $TEST_EXEC_DIR"
  exit 0
fi

echo "Found ${#test_executables[@]} tests to run."
echo "----------------------------------------"

# Disable job control to suppress crash messages
set +m

# Temporarily redirect stderr to suppress crash messages
exec 3>&2  # Save stderr to file descriptor 3
exec 2>/dev/null  # Redirect stderr to /dev/null

# Initialize counters
pass_count=0
fail_count=0

# Loop through each test executable
for executable in "${test_executables[@]}"; do
  test_name=$(basename "$executable")
  log_file="$LOG_DIR/${test_name}.log"

  # Restore stderr temporarily for our output
  exec 2>&3
  printf "Running ${test_name}...\\r"
  exec 2>/dev/null

  # Execute the test and redirect stdout and stderr to the log file
  "$executable" > "$log_file" 2>&1
  exit_code=$?

  # Strip ANSI escape characters from the log file
  sed -i 's/\x1b\[[0-9;]*m//g' "$log_file"

  # Restore stderr for our output
  exec 2>&3

  # Check the exit code of the last command
  if [ $exit_code -eq 0 ]; then
    printf "\033[42m\033[1m[PASS]\033[0m ${log_file}                          \n"
    ((pass_count++))
  else
    printf "\033[41m\033[1m[FAIL]\033[0m ${log_file}                         \n"
    ((fail_count++))
  fi
  exec 2>/dev/null
done

# Restore stderr permanently
exec 2>&3
exec 3>&-

# Re-enable job control
set -m

echo "----------------------------------------"
echo "Test run complete."
echo "Results: ${pass_count} passed, ${fail_count} failed"

# Exit with non-zero code if any tests failed
if [ $fail_count -gt 0 ]; then
  exit 1
else
  exit 0
fi
