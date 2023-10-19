#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>

namespace print{
    int __PRINT_LEVEL__ = 0;
    void __PL_UP__() {
        __PRINT_LEVEL__ += 1;
    }
    void __PL_DOWN__() {
        __PRINT_LEVEL__ -= 1;
    }
    std::string __LW__() {
        std::string lw(__PRINT_LEVEL__ * 2, ' ');
        return(lw);
    }

    template <typename T>
    void print(const T& x) {
        std::cout << __LW__() << x << std::endl;
    }
    void print(const std::string& x) {
        std::cout << __LW__() << "\"" << x << "\"" << std::endl;
    }

    template <typename tuple_elem_type>
    void print(const std::tuple<tuple_elem_type, tuple_elem_type>& x) {
        std::cout << __LW__() << "(" << std::endl;
        __PL_UP__();
        std::cout << __LW__() << "0" << std::endl;
        std::cout << __LW__() << ":" << std::endl;
        print(std::get<0>(x));
        std::cout << __LW__() << "," << std::endl;
        std::cout << __LW__() << "1" << std::endl;
        std::cout << __LW__() << ":" << std::endl;
        print(std::get<1>(x));
        __PL_DOWN__();
        std::cout << __LW__()  << ")" << std::endl;
    }
    template <typename map_key_type, typename map_elem_type>
    void print(const std::unordered_map<map_key_type, map_elem_type>& x) {
        std::cout << __LW__() << "{" << std::endl;
        __PL_UP__();
        std::size_t i = 0;
        std::size_t x_size = x.size();
        for (const auto& it : x) {
            print(it.first);
            std::cout << __LW__() << "->" << std::endl;
            print(it.second);
            i++;
            if (i < x_size) {
                std::cout << __LW__() << "," << std::endl;
            }
        }
        __PL_DOWN__();
        std::cout << __LW__() << "}" << std::endl;
    }
    
    template <typename elem_type>
    void print(const std::vector<elem_type>& x) {
        std::cout << __LW__() << "[" << std::endl;
        __PL_UP__();
        for (int i = 0; i < x.size(); i++) {
            print(x[i]);
            if (i < x.size() - 1) {
                std::cout << __LW__() << "," << std::endl;
            }
        }
        __PL_DOWN__();
        std::cout << __LW__() << "]" << std::endl;
    }
    

}

#endif
