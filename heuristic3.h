
#ifndef UNTITLED_HEURISTIC3_H
#define UNTITLED_HEURISTIC3_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>

template <std::size_t s, std::size_t n>
void get_best_string_heur3(std::array<std::array<float, s>, n>& profile,
                      std::vector<float>& best_remaining_scores,
                      std::string& best_string, float& best_score,
                      std::string& alphabet, int k=(int) n){
    std::vector<float>& best_kmer_scores = best_remaining_scores;
    for (int i = 0; i < n-k+1; i++) { // CHANGED
        float score = 0;
        for (int ik = 0; ik < k and i + ik <n; ik++) { // CHANGED
            std::array<float,s>& profile_column = profile[i + ik];
            typename std::array<float,s>::iterator max = std::max_element(profile_column.begin(), profile_column.end()); //std::vector<float>::iterator
            score += *max;
            if (i==0 || ik==k-1){//i% k == 0 || i >= n-k){ // CHANGED
                best_string[i + ik] = alphabet[std::distance(profile_column.begin(),max)]; // CHANGED
                best_score += *max;
            }
        }
        best_kmer_scores[i] = score; // first they are the best_kmer_scores
    }
    for (int i = 0; i < n-k+1; i++) { // CHANGED
        best_remaining_scores[i] = best_score - best_kmer_scores[i];
    }
}



template <std::size_t s, std::size_t n>
void enumerate_kmers_heuristic3(std::array<std::array<float, s>, n>& profile, std::string& alphabet, float T, int k,
                     std::unordered_map<std::string, float>& strings,  std::string& new_string,
                     std::vector<float>& score_cashe, int ik=0,
                     int i=0, std::string best_string={}) {
    // This is similar to the enumeration function in the exhaustive search, however,
    // now we also check that the kmers are not part of the best string.
    for (int a = 0; a < s; a++) {
        float score = score_cashe[ik] + profile[i+ik][a];
        if (score > T) { // See comment in exhaustive search.
            new_string[i + ik] = alphabet[a];
            if (ik == k - 1 or i + ik == n-1) { // or if i*k + ik < n, for the last k-mer.
                    strings[new_string] = score;
            } else {
                score_cashe[ik + 1] = score;
                enumerate_kmers_heuristic3(profile, alphabet, T, k, strings, new_string,  score_cashe,  (ik + 1), i, best_string);
            }
        }
    }
}

template <std::size_t s, std::size_t n>
auto heuristic3(std::array<std::array<float, s>, n>&  profile, std::string& alphabet, float T, int k=1){
    std::string best_string(n, ' ');
    std::vector<float> best_remaining_scores((int) n ); // CHANGED
    float best_score;
    get_best_string_heur3<s, n> (profile, best_remaining_scores, best_string, best_score, alphabet, k);
    std::unordered_map<std::string, float> strings;// CHANGED: should be a hash table now, because doubles are generated.
    strings[best_string] = best_score;
    for (int i = 0; i < n-k+1; i++) { // CHANGED
        int ik = 0;
        std::vector<float> cashe(k); cashe[0] = best_remaining_scores[i];
        std::string new_string = best_string;
        enumerate_kmers_heuristic3(profile, alphabet, T, k, strings,  new_string,  cashe, ik,  i, best_string);
    }
    return(strings);        // return 2 character arrays.
}


int test_heuristic3() {
    const std::size_t n = 3;
    const std::size_t k = 2;
    const std::size_t s = 4;

    std::array<std::array<float, s>, n> profile = {{{0.3, 0.2, 0.4, 0.1,},
                                                    {0.2, 0.25, 0.25, 0.3,},
                                                    {0.5, 0.1, 0.2, 0.2,}}};
//    -1.73697 -2.32193 -1.32193 -3.32193
//    -2.32193 -2 -2 -1.73697
//    -1 -3.32193 -2.32193 -2.32193
    profile = matrix_operation(profile, &std::log2f); // takes the logarithm
    std::string alphabet = {'A', 'C', 'G', 'T'};


    heuristic3 (profile, alphabet, -7, k);
    return 0;
}




#endif