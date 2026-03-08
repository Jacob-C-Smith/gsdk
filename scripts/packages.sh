#!/bin/bash

# parameters
SUFFIX=$1
OUT=$2

# packages
source ./scripts/modules.sh

# logs
printf "\033[96;1;4mgsdk memory tests\033[0m\n"

# core 
printf "\n\033[96;1mgsdk > core\033[0m\n"
for example in "${core[@]}"; do
  ./scripts/memcheck.sh $example$SUFFIX core 
done

# data 
printf "\n\033[96;1mgsdk > data\033[0m\n"
for example in "${data[@]}"; do
  ./scripts/memcheck.sh $example$SUFFIX data 
done

# reflection
printf "\n\033[96;1mgsdk > reflection\033[0m\n"
for example in "${reflection[@]}"; do
  ./scripts/memcheck.sh $example$SUFFIX reflection 
done

# performance
printf "\n\033[96;1mgsdk > performance\033[0m\n"
for example in "${performance[@]}"; do
  ./scripts/memcheck.sh $example$SUFFIX performance 
done
