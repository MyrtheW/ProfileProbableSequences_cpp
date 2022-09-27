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

float median(std::vector<float> v, std::size_t &s){
    size_t n = s / 2;
    nth_element(v.begin(), v.begin()+n, v.end());
    while (v[n] == -std::numeric_limits<float>::infinity()){
        n-=1;
        nth_element(v.begin(), v.begin()+n, v.end());
        }
    return v[n];
}

float sum_medians(std::vector<std::vector<float>>&  profile, std::size_t &n, std::size_t &s)
{   float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += median(profile[i], s);
    }
    return sum;
}

void read_matrix (std::string filename, std::vector<std::vector<float>>&  profile,std::size_t &n, std::size_t &s, std::string&  alphabet){
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
    }else{ std::cout << "File not found!";}
}


template <typename T>
std::string outstring(std::vector<T> out_array){
    std::string out = "";
    for (auto &item: out_array) { std::string x = std::to_string(item); out = out + x + ", "; }
    return out;
}

int evaluation_top_fraction(std::string folder= "C:\\Users\\myrth\\Documents\\SCHOOL\\1E SEMESTER MASTER\\S; stage\\Python code\\data\\DNA_profiles\\",
                            std::string file_name= "MA0007.1.jaspar.txt",
                            std::size_t n =  20, std::size_t s =  4, float T_perc=1.01) {
// SET PARAMETERS
    std::vector<int> bs = {1, 5,10, 20, 50, 1000, 2000}; // b must be larger than ... if you want to find ... candidates
    std::vector<int> ks = {1};
    for (int x = 5; x > 0; x--) ks.push_back(std::ceil(n / x));
    //std::log2(std::pow(0.53, n)); // Threshold , 0.53 for n=14 --> 267, 0.4 for n=8 --> 301 results, 0.3 for n=8, --> 3000 results
    // for n =20 0.53--> 2162. exhaustive time 0.017 s, 36500 results.


// READ FILE
// Read in the profile matrix and the alphabet
    std::cout << folder +  file_name <<std::endl;
    std::vector<std::vector<float>> profile(n, std::vector<float>(s));
    std::string alphabet;
    read_matrix(folder + file_name, profile, n,s, alphabet);
    alphabet_dict = create_alphabet_dict(alphabet);
    struct timespec start, end;
    // Get threshold
    std::vector<std::vector<float>> profile_copy = profile;
    std::string best_string(n, ' '); float best_score=0;
    get_best_string (profile_copy,n,s, best_string, best_score, alphabet, 1);
    float median = sum_medians(profile,n,s);
    float T = T_perc*(median - best_score) + best_score;

// EXHAUSTIVE
// Generate the exhaustive search results.
    clock_gettime(CLOCK_MONOTONIC, &start);
    auto result_exhaustive = exhaustive(profile,n,s, alphabet, T);
    clock_gettime(CLOCK_MONOTONIC, &end);
    float size_exhaustive = (std::get<0>(result_exhaustive)).size();
    double time_exhaustive = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    //std::sort(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());
    //std::reverse(std::get<1>(result_exhaustive).begin(), std::get<1>(result_exhaustive).end());

// HEURISTIC 1
    std::vector<float> found_fractions_h1 = {};
    std::vector<float> speedups_h1 ={};
    for (auto &k : ks) { //I cannot loop over this if k is in the template.
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result = heuristic1 (profile,n,s, alphabet, T, k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h1.push_back(time_exhaustive/time_result);
        found_fractions_h1.push_back((std::get<0>(result)).size()/size_exhaustive);
    }

    // HEURISTIC 2
    std::vector<float> found_fractions_h2 = {}; std::vector<float> speedups_h2 ={};
    for (auto &b : bs) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result = heuristic2(profile, n,s, alphabet, T, b);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h2.push_back(time_exhaustive/time_result);
        found_fractions_h2.push_back((std::get<0>(result)).size()/size_exhaustive);
    }

    // HEURISTIC 3
    std::vector<float> found_fractions_h3 = {};
    std::vector<float> speedups_h3 ={};
    for (auto &k : ks) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result = heuristic3 (profile, n,s, alphabet, T, k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h3.push_back(time_exhaustive/time_result);
        found_fractions_h3.push_back((result).size()/size_exhaustive);
    }


// SAVE RESULTS
    // Saving results to a python library, that can be read in to make figures.
    std::ofstream o(folder+"results\\"+file_name.substr(0,6) + "_topvsspeed.py"); // "here.py");//
    o << "file_name = \"" << file_name.substr(0,7) << "\"" <<std::endl;
    o << "n = " << n <<std::endl;
    o << "T = " << T <<std::endl;
    o << "size_exhaustive = " << size_exhaustive <<std::endl;
    o << "time_exhaustive = " << time_exhaustive <<std::endl;
    o << "ks = [" << outstring(ks) << "]" <<std::endl;
    o << "found_fractions_h1 = [" << outstring(found_fractions_h1) << "]" <<std::endl;
    o << "speedups_h1 = [" << outstring(speedups_h1) << "]" <<std::endl;
    o << "bs = [" << outstring(bs) << "]" <<std::endl;
    o << "found_fractions_h2 = [" << outstring(found_fractions_h2) << "]" <<std::endl;
    o << "speedups_h2 = [" << outstring(speedups_h2) << "]" <<std::endl;
    o << "found_fractions_h3 = [" << outstring(found_fractions_h3) << "]" <<std::endl;
    o << "speedups_h3 = [" << outstring(speedups_h3) << "]" <<std::endl;
    o.close();
    return 0;
    }

void distribution(std::string folder, std::string file_name, float T_perc=1.01,std::size_t n = 20, std::size_t s = 4) {
    // SET PARAMETERS
    std::size_t k = 5;
    int b = 20;

    // READ FILE
    // Read in the profile matrix and the alphabet
    std::vector<std::vector<float>> profile(n, std::vector<float>(s));
    std::string alphabet;
    read_matrix(folder + file_name, profile, n,s, alphabet);
    alphabet_dict = create_alphabet_dict(alphabet);
    // Get threshold
    std::vector<std::vector<float>> profile_copy = profile;
    std::string best_string(n, ' '); float best_score=0;
    get_best_string (profile_copy,n,s, best_string, best_score, alphabet, 1);
    float median = sum_medians(profile,n,s);
    float T = T_perc*(median - best_score) + best_score;//T_perc*best_score;
    struct timespec start, end;

    // HEURISTIC 3
    clock_gettime(CLOCK_MONOTONIC, &start);
    auto result_exhaustive = exhaustive(profile,n,s, alphabet, T);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_exhaustive = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    // HEURISTIC 3
    auto result_heuristic1 = heuristic1 (profile,n,s, alphabet, T, k);
    // HEURISTIC 3
    auto result_heuristic2 = heuristic2 (profile, n,s,alphabet, T, b);
    // HEURISTIC 3
    auto result_heuristic3 = heuristic3 (profile, n,s,alphabet, T, k);

    // WRITE FILE
    std::vector<float> result_heuristic3_values;
    for (auto item = result_heuristic3.begin(); item != result_heuristic3.end(); item++){
        result_heuristic3_values.push_back(std::get<1>(*item));
    }
    std::ofstream o(folder+"results\\"+file_name.substr(0,6) + "_distribution.py"); // "here.py");//
    o << "file_name = \"" << file_name.substr(0,7) << "\"" <<std::endl;
    o << "n = " << n <<std::endl;
    o << "T = " << T <<std::endl;
    o << "b = " << b <<std::endl;
    o << "k = " << k <<std::endl;
    o << "size_exhaustive = " << std::get<1>(result_exhaustive).size() <<std::endl;
    o << "time_exhaustive = " << time_exhaustive <<std::endl;
    o << "results_exhaustive = [" << outstring(std::get<1>(result_exhaustive)) << "]" <<std::endl;
    o << "results_h1 = [" << outstring(std::get<1>(result_heuristic1)) << "]" <<std::endl;
    o << "results_h2 = [" << outstring(std::get<1>(result_heuristic2)) << "]" <<std::endl;
    o << "results_h3 = [" << outstring(result_heuristic3_values) << "]" <<std::endl;
    o.close();
}
void evaluation_vsn(std::string folder, std::string file_name, std::size_t max_n = 20, float T_perc=1.01, std::size_t s = 4) {


//    float normalized_threshold = 0.4;
//    std::map<int,float> normalized_thresholds = {{5,0.29},  {10,0.42}, {15,0.53}, {20,0.54}};
// Threshold , 0.53 for n=14 --> 267, 0.4 for n=8 --> 301 results, 0.3 for n=8, --> 3000 results
    // for n =20 0.53--> 2162. exhaustive time 0.017 s, 36500 results.
    float k_factor = 0.25;
    struct timespec start, end;
    std::vector<float> times_exhaustive = {};
    std::vector<float> sizes_exhaustive = {};
    std::vector<float> found_fractions_h3 = {};
    std::vector<float> speedups_h3 ={};
    std::vector<float> found_fractions_h1 = {};std::vector<float> speedups_h1 ={};
    std::vector<float> found_fractions_h2 = {}; std::vector<float> speedups_h2 ={};
    std::vector<float> ns = {};
    std::vector<float> ks = {};
    std::vector<float> bs = {};
    std::vector<float> thresholds = {};
    for (std::size_t n = 5; n < max_n+1; n=n+5){
        ns.push_back(n);
        int k = k_factor*n;
        ks.push_back(k);
        int b = 20;
        bs.push_back(b);

        // READ FILE

        std::vector<std::vector<float>> profile(n, std::vector<float>(s));
        std::string alphabet;
        read_matrix(folder + file_name, profile, n,s, alphabet);
        alphabet_dict = create_alphabet_dict (alphabet);
        // get threshold
        std::vector<std::vector<float>> profile_copy = profile;
        std::string best_string(n, ' '); float best_score=0;
        get_best_string (profile_copy,n,s, best_string, best_score, alphabet, 1);
        float median = sum_medians(profile,n,s);
        float T = T_perc*(median - best_score) + best_score;//T_perc*best_score;
        thresholds.push_back(T);

        // EXHAUSTIVE
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result_exhaustive = exhaustive(profile,n,s, alphabet, T);
        clock_gettime(CLOCK_MONOTONIC, &end);
        float size_exhaustive = (std::get<0>(result_exhaustive)).size();
        double time_exhaustive = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        sizes_exhaustive.push_back(size_exhaustive);
        times_exhaustive.push_back(time_exhaustive);

        // HEURISTIC 1
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result = heuristic1 (profile,n,s, alphabet, T, k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h1.push_back(time_exhaustive/time_result);
        found_fractions_h1.push_back((std::get<0>(result)).size()/size_exhaustive);

        // HEURISTIC 2
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = heuristic2(profile, n,s, alphabet, T, b);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h2.push_back(time_exhaustive/time_result);
        found_fractions_h2.push_back((std::get<0>(result)).size()/size_exhaustive);

        // HEURISTIC 3
        clock_gettime(CLOCK_MONOTONIC, &start);
        auto result3 = heuristic3 (profile, n,s, alphabet, T, k);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_result = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        if (time_result != 0) speedups_h3.push_back(time_exhaustive/time_result);
        found_fractions_h3.push_back((result3).size()/size_exhaustive);
     }
    // SAVE RESULTS
    // Saving results to a python library, that can be read in to make figures.
    std::ofstream o(folder+"results\\"+file_name.substr(0,6) + "_vsn.py"); // "here.py");//
    o << "file_name = \"" << file_name.substr(0,7) << "\"" <<std::endl;
    o << "ns = [" << outstring(ns) << "]" <<std::endl;
    o << "thresholds = [" << outstring(thresholds) << "]"<<std::endl;
    o << "sizes_exhaustive = [" << outstring(sizes_exhaustive) << "]" <<std::endl;
    o << "times_exhaustive = [" << outstring(times_exhaustive) << "]" <<std::endl;
    o << "ks = [" << outstring(ks) << "]" <<std::endl;
    o << "found_fractions_h1 = [" << outstring(found_fractions_h1) << "]" <<std::endl;
    o << "speedups_h1 = [" << outstring(speedups_h1) << "]" <<std::endl;
    o << "bs = [" << outstring(bs) << "]" <<std::endl;
    o << "found_fractions_h2 = [" << outstring(found_fractions_h2) << "]" <<std::endl;
    o << "speedups_h2 = [" << outstring(speedups_h2) << "]" <<std::endl;
    o << "found_fractions_h3 = [" << outstring(found_fractions_h3) << "]" <<std::endl;
    o << "speedups_h3 = [" << outstring(speedups_h3) << "]" <<std::endl;
    o.close();

}
//float OFV(float x, float y){
//    // y --> delta t / t exact.
//    // x--> found fraction.
//    return(x/100 +y/2);
//}

//void multiple_matrices(){
//    for (folder in DNA, RNA, protein) //"MA0007.1.jaspar.txt"
//    for (in (, "RF00523.afa.txt.txt", "RF00005.afa.txt.txt")){
//        for (std::size_t n = 5; n < 20+1; n=n+5) {
//            read_matrix(filename,n,s)
//            get_best_string(profile)
//            for (auto T_perc in (1.01,1.05,1.1)){
//                T = best_score * T_perc;
//                evaluation_top_fraction(file_name+folder)
//            }
//        }
//    }
//}

#endif //UNTITLED_EVALUATION_H
