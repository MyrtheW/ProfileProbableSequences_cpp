
#ifndef UNTITLED_HEURISTIC1_H
#define UNTITLED_HEURISTIC1_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>


template <std::size_t s, std::size_t n, std::size_t k>
void get_best_string(std::array<std::array<float, s>, n>& profile,
                                    std::array<float, (int) n/k>& best_remaining_scores,
                                    std::array<char, n>& best_string, float& best_score, char alphabet[]){
    std::array<float, (int) n/k>& best_kmer_scores = best_remaining_scores;
//    for (int i = 0; i < 9; i++) {
//        std::cout << i;
//    }
    for (int i = 0; i < n/k; i++) {
        float score = 0;
        for (int ik = 0; ik < k; ik++) {
            std::array<float,s>& profile_column = profile[i*k + ik];
            //typename std::array<float,s>::iterator maxx;
            typename std::array<float,s>::iterator max = std::max_element(profile_column.begin(), profile_column.end()); //std::vector<float>::iterator
            score += *max;
            best_string[i*k + ik] = alphabet[std::distance(profile_column.begin(), max)]; //use this to index the alphabet.
        }
        best_kmer_scores[i] = score; // first they are the best_kmer_scores
    }
    best_score = std::accumulate(best_kmer_scores.begin(), best_kmer_scores.end(), 0);
    for (int i = 0; i < n/k; i++) {
        best_remaining_scores[i] = best_score - best_kmer_scores[i];
    }
}
template <std::size_t n>
bool check_in_best(std::array<char, n>& arr1, std::array<char, n>& arr2, int i, int i_end){
    for (i; i < i_end; i++)
        if (arr1[i] != arr2[i])
            return false;
return true;
}

template <std::size_t k, std::size_t s, std::size_t n>
void enumerate_kmers(std::array<std::array<float, s>, n>& profile, char alphabet[], float T,
                     std::vector<std::array<char, n>>& strings, std::vector<float>& scores,
                     int ik=0, std::array<float, k> cashe={0}, std::array<char, n> icashe={}, int i=0, std::array<char, n> best_string={}) {
    for (int a = 0; a < s; a++) {
        float score = cashe[ik] + profile[a][i+ik];
        if (score > T) {
            if (ik == k - 1) {
                icashe[i*k + ik] = alphabet[a];
//                std::array<char, n> string = most_probable_string
//                for i in alphabet
                if (not check_in_best(icashe, best_string, i*k, i*k +k)){// TODO: should only check this for the slice best_string[i:i+k]
                    strings.push_back(icashe); // should generate a copy... ;
                    // alternative is to only store i:i+k, and replace the "icashe" kmer in the most probable string each time the threshold is met.
                    scores.push_back(score);
                }
            } else {
                cashe[ik + 1] = score;
                icashe[i*k + ik] = alphabet[a];
                enumerate_kmers(profile, alphabet, T, strings, scores, (ik + 1), cashe, icashe, i, best_string);
            }
        }
    }
}

template <std::size_t s, std::size_t n, std::size_t k>
int heuristic1(std::array<std::array<float, s>, n>&  profile, char alphabet[], float T){
    std::array<char, n> best_string = {};
    std::array<float, (int) n/k> best_remaining_scores; // (int) std::ceil((float) n/k)
    float best_score;
    get_best_string<s, n, k> (profile, best_remaining_scores, best_string, best_score, alphabet);
    std::vector<std::array<char, n>> strings = {best_string};
    std::vector<float> scores = {best_score};
    for (int i = 0; i < n/k; i++) {
        // TODO :part_of_profile = point to part of the profile
        int ik = 0;
        //if (x > T) {
        std::array<float, k> cashe = {best_remaining_scores[i]};
        std::array<char, n> icashe = best_string; // should generate a copy...
        enumerate_kmers(profile, alphabet, T, strings, scores, ik, cashe, icashe, i, best_string);
    }
    return 0;        // return 2 character arrays.
}

int test_heuristic1() {
    const std::size_t n = 3;
    const std::size_t k = 1;
    const std::size_t s = 4;

    std::array<std::array<float, s>, n> profile = {{{0.3, 0.2, 0.4, 0.1,},
                                                    {0.2, 0.25, 0.25, 0.3,},
                                                    {0.5, 0.1, 0.2, 0.2,}}};
    profile = matrix_operation(profile, &std::log2f); // takes the logarithm
    char alphabet[s] = {'A', 'C', 'G', 'T'};


    heuristic1<s, n, k> (profile, alphabet, -7);
    return 0;
}




#endif //UNTITLED_HEURISTIC1_H
