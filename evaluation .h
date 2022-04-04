//
// Created by myrth on 01/04/2022.
//

#ifndef UNTITLED_EVALUATION_H
#define UNTITLED_EVALUATION_H
#include <chrono>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <queue>
#include "exhaustive.h"
#include "heuristic1.h"
#include "heuristic2.h"

auto found_fraction = [] (auto& size_exhaustive, auto& size_result){return size_result/size_exhaustive;};

auto speedup = [] (auto& time_exhaustive, auto& time_result)
{
    return time_result/time_exhaustive;
};

int evaluation() {
    const std::size_t n =  3;
    const std::size_t s = 4;
    std::string alphabet = {'A', 'C', 'G', 'T'};
    alphabet_dict = create_alphabet_dict<s> (alphabet);
    std::array<std::array<float, s>, n> profile = {{{0.3, 0.2, 0.4, 0.1,},
                                                    {0.2, 0.25, 0.25, 0.3,},
                                                    {0.5, 0.1, 0.2, 0.2,}}};
    const std::vector<std::size_t > ks = {1, 2,3,4,};
    const std::vector<int> bs = {1, 2,3,4,};
    float T = -7;
    clock_t start, end;
//------------------------
    start = clock();
    auto result_exhaustive = exhaustive(profile, alphabet, T);
    end = clock();
    std::size_t size_exhaustive = std::get<0>(*result_exhaustive).size();
    clock_t time_exhaustive = end - start;
//-------------------------
    for (const auto &k : ks) {
        start = clock();
        const std::size_t k2 = 2;
        auto result = heuristic1<s, n, k2> (profile, alphabet, T);
        end = clock();
        clock_t time_result = end - start;
        clock_t speedup = time_result/time_exhaustive;
        float found_fraction = std::get<0>(*result).size()/size_exhaustive;
    }
    for (auto &b : bs) {
        start = clock();
        auto result = heuristic2<s, n, n> (profile, alphabet, T, b);
        end = clock();
        clock_t time_result = end - start;
        clock_t speedup = time_result/time_exhaustive;
        float found_fraction = result->size()/size_exhaustive;
    }
    return 0;

}
#endif //UNTITLED_EVALUATION_H
