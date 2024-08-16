#include "sample_head.h"
#include <iostream>
#include <vector>
#include <string>

int main() {

    std::vector<int> numbers = {1, 2, 3,4,5};
    int vectorSum = processVector(numbers);
    std::cout << "Sum of numbers: " << vectorSum << std::endl;

    return 0;
}
