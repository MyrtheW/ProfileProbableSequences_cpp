
#ifndef UNTITLED_HEURISTIC1_H
#define UNTITLED_HEURISTIC1_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>

template <std::size_t s, std::size_t n>
void get_best_string(std::array<std::array<float, s>, n>& profile,
                     std::string& best_string, float& best_score,
                     std::string& alphabet, int k=(int) 1){
    for (int i = 0; i < n; i++) {
        auto max = std::max_element(profile[i].begin(), profile[i].end());
        float max_score = *max;
        best_score += max_score;
        best_string[i] = alphabet[std::distance(profile[i].begin(), max)];
        //for (int a = 0; a < s; a++){ profile[i][a] -= max_score;}
        }
}



bool check_in_best(std::string& arr1, std::string& arr2, int i, int i_end){
    for (i; i < i_end; i++)
        if (arr1[i] != arr2[i])
            return false;
return true;
}


template <std::size_t s, std::size_t n>
void enumerate_kmers(std::array<std::array<float, s>, n>& profile, std::string& alphabet, float T, int k,
                     std::vector<std::string>& strings, std::vector<float>& scores, std::string& new_string,
                      std::vector<float>& score_cashe, int i=0, int ik=0) {
    // This is similar to the enumeration function in the exhaustive search, however,
    // now we also check that the kmers are not part of the best string.
    for (int a = 0; a < s; a++) {
        float score = score_cashe[ik] + profile[i*k+ik][a]; // Check if this should not be i*k+ik.
        if (score > T) { // See comment in exhaustive search.
            new_string[i*k + ik] = alphabet[a];
            if (ik == k - 1 or i*k + ik == n-1) { // or if i*k + ik < n, for the last k-mer.
                if (not check_in_best(new_string, strings[0], i * k,i * k + k)) {
                    strings.push_back(new_string);
                    scores.push_back(score + scores[0]);
                }
            } else {
                score_cashe[ik + 1] = score;
                enumerate_kmers(profile, alphabet, T, k, strings, scores, new_string, score_cashe, i, (ik + 1));
            }
        }
    }
}



template <std::size_t s, std::size_t n>
auto heuristic1(std::array<std::array<float, s>, n>  profile, std::string& alphabet, float T, int k=1){ //CHANGE
    std::string best_string(n, ' '); float best_score;
    get_best_string<s, n> (profile, best_string, best_score, alphabet, k);
    std::vector<std::string> strings = {best_string};
    T -= best_score;
    std::vector<float> scores = {best_score};
    for (int i = 0; i < std::ceil( (float)n/k); i++) { // For each k-sized column in the profile matrix.
        std::vector<float> score_cashe(k); score_cashe[0] = {0}; //CHANGE //best_remaining_scores[i]};
        std::string new_string = best_string; // The new string starts with a copy of the best string.
        enumerate_kmers(profile, alphabet, T, k, strings, scores, new_string,  score_cashe,  i, 0);
    }
    return std::make_tuple(std::move(strings), std::move(scores));
}



#endif //UNTITLED_HEURISTIC1_H
