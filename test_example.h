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




// some printing functions:
template <typename T>
void print(T a) {
    std::cout << a;
    std::cout << ", ";
}

void print(std::string a) {
    std::cout << "\"";
    std::cout << a;
    std::cout << "\", ";
}
template <typename T>
void print(std::vector <T> const &a) {
    std::cout << '{';
    for(int i=0; i < a.size(); i++)
        print(a.at(i));
    std::cout << '}';
}

struct hash_tuples_comparator {
    bool operator() (const std::pair<std::string, float>& tuple1,
                     const std::pair<std::string, float>& tuple2) const{
        return (std::get<1>(tuple1) > std::get<1>(tuple2));
    }
};
float plus_one(float x){
    return (x+2);
}
void test_example() {
    const std::size_t n = 4;
    const std::size_t s = 4;
    float T = -6.5;//-12;//-6.64; equals =0.01 normalized
    int b = 2;//256;
    std::size_t k = 1;

    // this is a small example matrix, that I use in my illustrations
    std::array<std::array<float, s>, n> profile = {{{0.3, 0.2, 0.4, 0.1,},
                                                    {0.2, 0.25, 0.25, 0.3,},
                                                    {0.5, 0.1, 0.2, 0.2,},
                                                    {0.4, 0.4, 0.1, 0.1,}}};
    //    -1.73697      -2.32193     -1.32193 -3.32193
    //    -2.32193      -2          -2          -1.73697
    //    -1            -3.32193 -2.32193 -2.32193


    profile = matrix_operation(profile, &std::log2f); // takes the logarithm
    //profile = matrix_operation(profile, plus_one); T +=8;
    std::string alphabet = {'A', 'C', 'G', 'T'};
    alphabet_dict = create_alphabet_dict<s> (alphabet);

    // EXHAUSTIVE
    auto result_exhaustive = exhaustive(profile, alphabet, T);
//    std::sort(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());
//    std::reverse(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());
//    for (auto score = std::get<1>(result_exhaustive).begin(); score != std::get<1>(result_exhaustive).end(); score++){
//        *score = std::pow(2, *score);
//    }

//    reference_exhaustive = {"AAAA", "AAAC", "ACAA", "ACAC", "AGAA", "AGAC", "ATAA", "ATAC", "CTAA", "CTAC", "GAAA", "GAAC", "GCAA", "GCAC", "GGAA", "GGAC", "GTAA", "GTAC", }; //length 18
//    reference_exhaustive_scores ={-6.38082, -6.38082, -6.05889, -6.05889, -6.05889, -6.05889, -5.79586, -5.79586, -6.38082, -6.38082, -5.96578, -5.96578, -5.64386, -5.64386, -5.64386, -5.64386, -5.38082, -5.38082, };
    //equals: {0.024 0.024 0.02 0.02 0.02 0.02 0.018 0.018 0.016 0.016 0.015 0.015 0.015 0.015 0.012 0.012 0.012 0.012 }
//    reference_heur1 = {"GTAA", "AAAA", "ACAA", "AGAA", "ATAA", "CTAA", "GAAA", "GCAA", "GGAA", "GTAC", };
//    {-5.38082, -6.38082, -6.05889, -6.05889, -5.79586, -6.38082, -5.96578, -5.64386, -5.64386, -5.38082, };
//    reference_heur2 = {"GAAA", "CTAC", "GAAC", "CTAA", "GGAA", "GCAC", "ATAC", "ATAA", "GTAC", "GGAC", "GCAA", "GTAA", };
//    -5.96578, -6.38082, -5.96578, -6.38082, -5.64386, -5.64386, -5.79586, -5.79586, -5.38082, -5.64386, -5.64386, -5.38082,
//    reference_heur3 = {"GGAA", "GAAA", "CTAA", "ATAA", "GTAC", "ACAA", "AGAA", "AAAA", "GCAA", "GTAA", }
//    -5.64386, -5.96578, -6.38082, -5.79586, -5.38082, -6.05889, -6.05889, -6.38082, -5.64386, -5.38082,


    // HEURISTIC 1
    auto result_heuristic1 = heuristic1 (profile, alphabet, T, k);

    // HEURISTIC 2
    auto result_heuristic2 = heuristic2 (profile, alphabet, T, b);
    //std::sort(result_heuristic2.begin(), result_heuristic2.end(), hash_tuples_comparator());

    auto result_heuristic3 = heuristic3 (profile, alphabet, T, k); // For k=2; length should be that of heuristic 1, +9. For k=3, length should be
    //std::sort(result_heuristic2.begin(), result_heuristic2.end(), hash_tuples_comparator());


    //PRINTING OUTPUT
    print(std::get<0>(result_exhaustive)); std::cout<<std::endl;
    print(std::get<1>(result_exhaustive)); std::cout<<std::endl;
    print(std::get<0>(result_heuristic1)); std::cout<<std::endl;
    print(std::get<1>(result_heuristic1)); std::cout<<std::endl;
    for (auto item = result_heuristic2.begin(); item != result_heuristic2.end(); item++){
        print(std::get<0>(*item));
    }std::cout<<std::endl;
    for (auto item = result_heuristic2.begin(); item != result_heuristic2.end(); item++){
        print(std::get<1>(*item));
    }std::cout<<std::endl;
    for (auto item = result_heuristic3.begin(); item != result_heuristic2.end(); item++){
        print(std::get<0>(*item));
    }std::cout<<std::endl;
    for (auto item = result_heuristic3.begin(); item != result_heuristic2.end(); item++){
        print(std::get<1>(*item));
    }std::cout<<std::endl;
}

#endif //UNTITLED_TEST_EXAMPLE_H
