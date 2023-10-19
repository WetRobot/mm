# Using in your project

This project is header-only and you can simply include it as a subdir of your
own project.

```c++
// assuming you have the project dir of "ngm" as a subdir of your own project
#include "./ngm/include/ngm/ngm.hpp"
```

# Running unit tests

```sh
mkdir build
cmake -G "Ninja" -S . -B build
cmake --build build
./build/tests.exe
```