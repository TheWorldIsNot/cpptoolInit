
#include "sample_head.h"
#include <iostream>
#include <vector>

int add(int a, int b) {
    return a + b;
}

int processVector(const std::vector<int>& c) {
    int sum=0;
    for(auto & tmp: c)
        sum+=tmp;
    return sum;
}

