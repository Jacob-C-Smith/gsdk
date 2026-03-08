#!/bin/bash

# parameters
EXAMPLE=$1
OUT=$2

# logs
printf "\033[44;1m[TEST]\033[0m %s\r" $EXAMPLE

# run valgrind
valgrind \
--leak-check=full \
--show-leak-kinds=all \
--track-origins=yes \
--track-fds=yes \
--error-exitcode=1 \
--log-file=output/valgrind/$OUT/$EXAMPLE \
./build/examples/$EXAMPLE > /dev/null 2>&1

# save the exit code
RESULT=$?

# check the exit code
if [ $RESULT -ne 0 ]; then
    printf "\033[41;1m[FAIL]\033[0m %s\n" $EXAMPLE
else
    printf "\033[42;1m[PASS]\033[0m %s\n" $EXAMPLE
fi

# propegate the exit code
exit $RESULT