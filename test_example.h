#ifndef UNTITLED_TEST_EXAMPLE_H
#define UNTITLED_TEST_EXAMPLE_H
//#include <iostream>
#include <vector>
#include <chrono>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <queue>
#include <fstream>
#include "exhaustive.h"
#include "heuristic1.h"
#include "heuristic2.h"
#include "heuristic3.h"





struct hash_tuples_comparator {
    bool operator() (const std::pair<std::string, float>& tuple1,
                     const std::pair<std::string, float>& tuple2) const{
        return (std::get<1>(tuple1) > std::get<1>(tuple2));
    }
};

void test_example() {
    const std::size_t n = 4;
    const std::size_t s = 4;
    float T = -12;//-6.64; // equals =0.01
    int b = 1;//256;
    std::size_t k = 2;

    // this is a small example matrix, that I use in my illustrations
    std::array<std::array<float, s>, n> profile = {{{0.3, 0.2, 0.4, 0.1,},
                                                    {0.2, 0.25, 0.25, 0.3,},
                                                    {0.5, 0.1, 0.2, 0.2,},
                                                    {0.25, 0.25, 0.25, 0.25,}}};
    profile = matrix_operation(profile, &std::log2f); // takes the logarithm
    std::string alphabet = {'A', 'C', 'G', 'T'};
    alphabet_dict = create_alphabet_dict<s> (alphabet);

    // EXHAUSTIVE
    auto result_exhaustive = exhaustive(profile, alphabet, T);
    std::sort(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());
    std::reverse(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());
    for (auto score = std::get<1>(result_exhaustive).begin(); score != std::get<1>(result_exhaustive).end(); score++){
        *score = std::pow(2, *score);
    }

    // HEURISTIC 1
    auto result_heuristic1 = heuristic1 (profile, alphabet, T, k);
    std::sort(std::get<1>(result_heuristic1).begin(), std::get<1>(result_heuristic1).end());
    std::reverse(std::get<1>(result_heuristic1).begin(), std::get<1>(result_heuristic1).end());
    for (auto score = std::get<1>(result_heuristic1).begin(); score != std::get<1>(result_heuristic1).end(); score++){
        *score = std::pow(2, *score);
    }
    // HEURISTIC 2

    auto result_heuristic2 = heuristic2 (profile, alphabet, T, b);
    //std::sort(result_heuristic2.begin(), result_heuristic2.end(), hash_tuples_comparator());

    auto result_heuristic3 = heuristic3 (profile, alphabet, T, k); // For k=2; length should be that of heuristic 1, +9. For k=3, length should be
    //std::sort(result_heuristic2.begin(), result_heuristic2.end(), hash_tuples_comparator());
    std::cout << 1*1;
}

#endif //UNTITLED_TEST_EXAMPLE_H
