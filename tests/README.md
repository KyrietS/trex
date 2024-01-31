# Trex unit tests
This directory contains unit tests for Trex.

## Running tests
From the root of the repository, run the following commands:
```
cmake -S tests -B build_tests
cmake --build build_tests
ctest --test-dir build_tests
```