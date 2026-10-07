#include "vector.h"
#include <iostream>

int main() {
    TVector<int> my_vec(8);
    int val = 1;

    //with regular iterator
    for (TVector<int>::iterator it = my_vec.begin(); it != my_vec.end(); ++it) {
        *it = val++;
    }

    //with const iterator
    for (TVector<int>::const_iterator it = my_vec.cbegin(); it != my_vec.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // its the same as first option
    // for (auto& x : my_vec)  x = val++;
    // for (const auto& x : my_vec)  std::cout << x << " ";

    return 0;
}