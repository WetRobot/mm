# What's this for?

You can fit and evaluate a Markov model. Evaluation of
log probability mass function has been parallelised with OpenMP.

```c++
#include <string>
#include "./include/mm/mm.hpp"

int main() {
    mm::MarkovModel<std::string> M(1.0, 1.0, true);
    M.update("a", "b");
    M.update("a", "a");
    M.update("a", "a");
    M.print();
    mm::print::print(M.lpmf("a", "x"));
    mm::print::print(M.lpmf("a", "a"));
    return(0);
}
```

# Using in your project

This project is header-only and you can simply include it as a subdir of your
own project.

```c++
// assuming you have the project dir of "mm" as a subdir of your own project
#include "./mm/include/mm/mm.hpp"
```

# Running unit tests

```sh
mkdir build
cmake -G "Ninja" -S . -B build
cmake --build build
./build/tests.exe
```