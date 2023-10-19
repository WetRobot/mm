# What's this for?

You can fit and evaluate a character ngram model. Evaluation of
log probability mass function has been parallelised with OpenMP.

```c++
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "./include/ngm/ngm.hpp"
int main() {
    ngram::NgramModel M(3, 1.0, 1.0, true);
    // fit model
    M.update(std::string("<<a>"));
    M.update(std::string("<<b>"));
    // evaluate on holdout data
    std::vector<double> lp(10);
    std::vector<std::string> x(10);
    for (int i = 0; i < x.size(); i++) {
        x[i] = "<<aaaaaaaa>";
    }
    // use two threads
    M.lpmf(x, lp, 2);
    ngram::print::print(lp);
    return(0);
}
```

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