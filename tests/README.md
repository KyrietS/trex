# Trex unit tests
This directory contains unit tests for Trex.

## Running tests
From the root of the repository, run the following commands:
```
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```