
#ifndef UNTITLED_HEURISTIC1_H
#define UNTITLED_HEURISTIC1_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>

 // not functional anymore after converting character arrays to strings.
template <std::size_t s, std::size_t n, std::size_t k>
void get_best_string(std::array<std::array<float, s>, n>& profile,
                                    std::array<float, (int) n/k +1>& best_remaining_scores,
                                    std::string& best_string, float& best_score,
                                    std::string& alphabet){
    std::array<float, (int) n/k +1>& best_kmer_scores = best_remaining_scores;
    for (int i = 0; i < std::ceil( (float) n/k); i++) {
        float score = 0;
        for (int ik = 0; ik < k and i*k + ik <n; ik++) { // TODO: make it work if n%k != 0, try with: i*k + ik <n
            std::array<float,s>& profile_column = profile[i*k + ik];
            typename std::array<float,s>::iterator max = std::max_element(profile_column.begin(), profile_column.end()); //std::vector<float>::iterator
            score += *max;
            best_string[i*k + ik] = alphabet[std::distance(profile_column.begin(), max)]; //use this to index the alphabet.
        }
        best_kmer_scores[i] = score; // first they are the best_kmer_scores
    }
    best_score = std::accumulate(best_kmer_scores.begin(), best_kmer_scores.end(), 0.0);
    for (int i = 0; i < std::ceil( n/k); i++) {
        best_remaining_scores[i] = best_score - best_kmer_scores[i];
    }
}
bool check_in_best(std::string& arr1, std::string& arr2, int i, int i_end){
    for (i; i < i_end; i++)
        if (arr1[i] != arr2[i])
            return false;
return true;
}

template <std::size_t k, std::size_t s, std::size_t n>
void enumerate_kmers(std::array<std::array<float, s>, n>& profile, std::string& alphabet, float T,
                     std::vector<std::string>& strings, std::vector<float>& scores, std::string& new_string,
                     int ik=0, std::array<float, k> cashe={0},
                     int i=0, std::string best_string={}) { // ALTERNATIVE extra parameter, bool new_equals_best = 1
    // std::string& best_string = strings[0];
    for (int a = 0; a < s; a++) {
        float score = cashe[ik] + profile[i+ik][a];
        if (score > T) { // By placing the threshold here, it can already prune a set of k-mers, if the first nucleotides in the k-mer would already cause the score to not reach the threshold
            if (ik == k - 1 or i*k + ik == n-1) { // or if i*k + ik < n, for the last k-mer.
                new_string[i*k + ik] = alphabet[a];
                if (not check_in_best(new_string, best_string, i*k, i*k +k)){ //ATLERNATIVE: if not a_equals_best AND new_equals_best
                    strings.push_back(new_string); // should generate a copy, but that seems to happen.
                    // alternative is to only store i:i+k, and replace the "new_string" kmer in the most probable string each time the threshold is met:
                    //                std::string string = most_probable_string
                    //                     for i in alphabet
                    scores.push_back(score);
                }
            } else {
                cashe[ik + 1] = score;
                // ATLERNATIVE: if not a_equals_best
                new_string[i*k + ik] = alphabet[a]; // in some cases new_string[i*k + ik] already contains alphabet[a], hence the alternative, for "check_in_best"
                // You could improve the algorithm for some of the cases, by not storing these "new_strings" and "new_string" scores, (cashe),
                enumerate_kmers(profile, alphabet, T, strings, scores, new_string, (ik + 1), cashe,  i, best_string);
            }
        }
    }
}

template <std::size_t s, std::size_t n, std::size_t k>
int heuristic1(std::array<std::array<float, s>, n>&  profile, std::string& alphabet, float T){
    std::string best_string(n, ' ');
    std::array<float, (int) n/k + 1> best_remaining_scores; // (int) std::ceil((float) n/k)
    float best_score;
    get_best_string<s, n, k> (profile, best_remaining_scores, best_string, best_score, alphabet);
    std::vector<std::string> strings = {best_string};
    std::vector<float> scores = {best_score};
    for (int i = 0; i < std::ceil( (float)n/k); i++) { // for each profile column
        int ik = 0;
        //if (x > T) {
        std::array<float, k> cashe = {best_remaining_scores[i]};
        std::string new_string = best_string; // should generate a copy...
        enumerate_kmers(profile, alphabet, T, strings, scores, new_string, ik, cashe,  i, best_string);
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
    std::string alphabet = {'A', 'C', 'G', 'T'};


    heuristic1<s, n, k> (profile, alphabet, -7);
    return 0;
}




#endif //UNTITLED_HEURISTIC1_H
