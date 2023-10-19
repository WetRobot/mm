#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "./include/ngm/ngm.hpp"
// remember -fopenmp when you compile this. e.g.
// g++ -fopenmp examples/ex_01.cpp -o examples/ex_01.o
int main() {
    ngram::NgramModel M(3, 1.0, 1.0, true);
    M.update(std::string("<<a>"));
    M.update(std::string("<<b>"));
    M.print();
    std::vector<double> lp(10);
    std::vector<std::string> x(10);
    for (int i = 0; i < x.size(); i++) {
        x[i] = "<<aaaaaaaa>";
    }
    M.lpmf(x, lp, 2);
    ngram::print::print(lp);
    return(0);
}
