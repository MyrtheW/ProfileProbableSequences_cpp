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
#include <fstream>
#include "exhaustive.h"
#include "heuristic1.h"
#include "heuristic2.h"

template <std::size_t s, std::size_t n>
void read_matrix (std::string filename, std::array<std::array<float, s>, n>&  profile, std::string&  alphabet){
    std::ifstream f(filename, std::ios::in);
    if ( f.is_open() ) { // always check whether the file is open
        int n_file; int s_file;
        f >> n_file  >>  s_file;
        if ((n_file < n) || (s_file != s)) std::cout << "Matrix dimensions!";
        f >> alphabet;
        for (int i = 0; i < n; i++){
            for (int j = 0; j<s; j++){
                std::string x;
                f >> x;
                float x_new;
                if (x == "-inf"){ x_new = -std::numeric_limits<float>::infinity();}
                else{ x_new = std::stof(x);}
                profile[i][j] = x_new;
            }
    }
    }
}


template <typename T>
std::string outstring(std::vector<T> out_array){
    std::string out = "";
    for (auto &item: out_array) { std::string x = std::to_string(item); out = out + x + ", "; }
    return out;
}

int evaluation() {
// SET PARAMETERS
    std::string file_name = "MA0007.1.jaspar.txt";
    const std::vector<int> ks = {1, 2,3,4,};
    const std::vector<int> bs = {1, 2,3,4,};
    float T = 9; // Threshold
    const std::size_t n =  12;

// READ FILE
// Read in the profile matrix and the alphabet
    std::string folder = "C:\\Users\\myrth\\Documents\\SCHOOL\\1E SEMESTER MASTER\\S; stage\\Python code\\data\\DNA_profiles\\";
    std::cout << folder + file_name <<std::endl;
    const std::size_t s = 4;
    std::array<std::array<float, s>, n> profile;
    std::string alphabet;
    read_matrix(folder + file_name, profile, alphabet);
    alphabet_dict = create_alphabet_dict<s> (alphabet);
    struct timespec start, end;

// EXHAUSTIVE
// Generate the exhaustive search results.
    clock_gettime(CLOCK_MONOTONIC, &start);
    auto result_exhaustive = exhaustive(profile, alphabet, T);
    clock_gettime(CLOCK_MONOTONIC, &end);
    float size_exhaustive = (std::get<0>(result_exhaustive)).size();
    double time_exhaustive = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    std::sort(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());
    std::reverse(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());

// HEURISTIC 1
    std::vector<float> found_fractions_h1 = {};
    std::vector<float> speedups_h1 ={};
    for (auto &k : ks) { //I cannot loop over this if k is in the template.
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result = heuristic1 (profile, alphabet, T, k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h1.push_back(time_exhaustive/time_result);
        found_fractions_h1.push_back((std::get<0>(result)).size()/size_exhaustive);
    }

// HEURISTIC 2
    std::vector<float> found_fractions_h2 = {};
    std::vector<float> speedups_h2 ={};
    for (auto &b : bs) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result = heuristic2 (profile, alphabet, T, b);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h2.push_back(time_exhaustive/time_result);
        found_fractions_h2.push_back((result).size()/size_exhaustive);
    }

//    // HEURISTIC 3
//    std::vector<float> found_fractions_h3 = {};
//    std::vector<float> speedups_h3 ={};
//    for (auto &k : ks) {
//        clock_gettime(CLOCK_MONOTONIC, &start);
//        auto result = heuristic3 (profile, alphabet, T, k);
//        clock_gettime(CLOCK_MONOTONIC, &end);
//        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
//        if (time_result != 0) speedups_h3.push_back(time_exhaustive/time_result);
//        found_fractions_h3.push_back((result).size()/size_exhaustive);
//    }

// SAVE RESULTS
    // Saving results to a python library, that can be read in to make figures.
    std::ofstream o(folder+"results\\"+file_name.substr(0,6) + ".py"); // "here.py");//
    o << "size_exhaustive = " << size_exhaustive <<std::endl;
    o << "ks = [" << outstring(ks) << "]" <<std::endl;
    o << "found_fractions_h1 = [" << outstring(found_fractions_h1) << "]" <<std::endl;
    o << "speedups_h1 = [" << outstring(speedups_h1) << "]" <<std::endl;
    o << "bs = [" << outstring(bs) << "]" <<std::endl;
    o << "found_fractions_h2 = [" << outstring(found_fractions_h2) << "]" <<std::endl;
    o << "speedups_h2 = [" << outstring(speedups_h2) << "]" <<std::endl;
    o.close();
    return 0;
    }



#endif //UNTITLED_EVALUATION_H
