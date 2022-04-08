
#ifndef UNTITLED_HEURISTIC3_H
#define UNTITLED_HEURISTIC3_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>

//template <std::size_t s, std::size_t n>
//void get_best_string_concat(std::array<std::array<float, s>, n>& profile,
//                     std::vector<float>& best_remaining_scores,
//                     std::string& best_string, float& best_score,
//                     std::string& alphabet, int k=(int) n,
//                     bool slide = 1){
//    std::vector<float>& best_kmer_scores = best_remaining_scores;
//    for (int i = 0; i < std::ceil( (float) n/k*(slide*(k - 2)+1)); i++) { // CHANGED If slide is true, then i<n-k. Else i<ceil(n/k)
//        float score = 0;
//        for (int ik = 0; ik < k and i*k/(slide*(k - 1)+1) + ik <n; ik++) { // CHANGED
//            std::array<float,s>& profile_column = profile[i*k/ (slide * (k - 1) + 1)  + ik];
//            typename std::array<float,s>::iterator max = std::max_element(profile_column.begin(), profile_column.end()); //std::vector<float>::iterator
//            score += *max;
//            if (i*k/(slide*(k - 1)+1) % k == 0){ // CHANGED
//                best_string[i * k / (slide * (k - 1) + 1) + ik] = alphabet[std::distance(profile_column.begin(),
//                                                                                         max)]; // CHANGED
//            }
//        }
//        best_kmer_scores[i] = score; // first they are the best_kmer_scores
//    }
//    best_score = std::accumulate(best_kmer_scores.begin(), best_kmer_scores.end(), 0.0);
//    for (int i = 0; i < std::ceil( (float) n/k*(slide*(k - 2)+1)); i++) { // CHANGED
//        best_remaining_scores[i] = best_score - best_kmer_scores[i];
//    }
//}

template <std::size_t s, std::size_t n>
void get_best_string2(std::array<std::array<float, s>, n>& profile,
                      std::vector<float>& best_remaining_scores,
                      std::string& best_string, float& best_score,
                      std::string& alphabet, int k=(int) n,
                      bool slide = 1){
    std::vector<float>& best_kmer_scores = best_remaining_scores;
    for (int i = 0; i < n-k+1; i++) { // CHANGED
        float score = 0;
        for (int ik = 0; ik < k and i + ik <n; ik++) { // CHANGED
            std::array<float,s>& profile_column = profile[i + ik];
            typename std::array<float,s>::iterator max = std::max_element(profile_column.begin(), profile_column.end()); //std::vector<float>::iterator
            score += *max;
            if ((i - 1)% k == 0 || i ==0){ // CHANGED
                best_string[i + ik] = alphabet[std::distance(profile_column.begin(),max)]; // CHANGED
            }
        }
        best_kmer_scores[i] = score; // first they are the best_kmer_scores
    }
    best_score = std::accumulate(best_kmer_scores.begin(), best_kmer_scores.end(), 0.0);
    for (int i = 0; i < n-k+1; i++) { // CHANGED
        best_remaining_scores[i] = best_score - best_kmer_scores[i];
    }
}

template <std::size_t s, std::size_t n>
void enumerate_kmers(std::array<std::array<float, s>, n>& profile, std::string& alphabet, float T, int k,
                     std::unordered_map<std::string, float>& strings,  std::string& new_string,
                     std::vector<float>& cashe, int ik=0,
                     int i=0, std::string best_string={}, bool slide=0) { // ALTERNATIVE extra parameter, bool new_equals_best = 1
    // std::string& best_string = strings[0];
    for (int a = 0; a < s; a++) {
        std::cout << cashe[ik] << std::endl;
        float score = cashe[ik] + profile[i+ik][a]; // WARNING: Segmentation fault
        //if (score > T) { // ATTENTION: By placing the threshold here, it can already prune a set of k-mers, if the first nucleotides in the k-mer would already cause the score to not reach the threshold
            if (ik == k - 1 or i*k + ik == n-1) { // or if i*k + ik = n - 1, for the last k-mer.
                if (score > T) {
                    new_string[i * k + ik] = alphabet[a];
                    if (not check_in_best(new_string, best_string, i * k,
                                          i * k + k)) { //ATLERNATIVE: if not a_equals_best AND new_equals_best
                        strings[new_string] = score; // CHANGED
                    }
                }
            } else {
                cashe[ik + 1] = score;
                // ATLERNATIVE: if not a_equals_best
                new_string[i*k + ik] = alphabet[a]; // in some cases new_string[i*k + ik] already contains alphabet[a], hence the alternative, for "check_in_best"
                // You could improve the algorithm for some of the cases, by not storing these "new_strings" and "new_string" scores, (cashe),
                enumerate_kmers(profile, alphabet, T, k, strings, new_string,  cashe,  (ik + 1), i, best_string);
            }
        //}
    }
}

template <std::size_t s, std::size_t n>
auto heuristic3(std::array<std::array<float, s>, n>&  profile, std::string& alphabet, float T, int k=1){
    std::string best_string(n, ' ');
    std::vector<float> best_remaining_scores((int) n ); // CHANGED
    float best_score;
    get_best_string2<s, n> (profile, best_remaining_scores, best_string, best_score, alphabet, k, 1);
    std::unordered_map<std::string, float> strings;
    strings[best_string] = best_score;
    for (int i = 0; i < n-k; i++) { // CHANGED // Signal: SIGTRAP (Trace/breakpoint trap)
        int ik = 0;
        std::cout << i << std::endl;
        //if (x > T) {
        std::vector<float> cashe(k); cashe[0] = best_remaining_scores[i];
        std::string new_string = best_string; // CHANGE: should be a hash table now.
        enumerate_kmers(profile, alphabet, T, k, strings,  new_string,  cashe, ik,  i, best_string);
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