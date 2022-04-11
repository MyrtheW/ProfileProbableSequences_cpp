
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
                                    std::vector<float>& best_remaining_scores,
                                    std::string& best_string, float& best_score,
                                    std::string& alphabet, int k=(int) 1){
    std::vector<float>& best_kmer_scores = best_remaining_scores;
    // The best_kmer_scores contain for every k-sized column, what the score is of the best k-mer in this column.
    // Afterwards, best_kmer_scores are used to calculate the best_remaining_scores, which contain for each column what
    // the total best score is, minus the best score for that column. This is used to quickly calculate the score for
    // new strings that are created by replacing k-mers in the best string.
    for (int i = 0; i < std::ceil( (float) n/k); i++) {
        float score = 0;
        for (int ik = 0; ik < k and i*k + ik <n; ik++) { // If n%k != 0, there will be a smaller k-mer at the end. Hence, i*k + ik <n
            std::array<float,s>& profile_column = profile[i*k + ik];
            auto max = std::max_element(profile_column.begin(), profile_column.end());
            score += *max;
            best_string[i*k + ik] = alphabet[std::distance(profile_column.begin(), max)];
            // The best scoring k-mers are used to generate the best overall string.
        }
        best_kmer_scores[i] = score;
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


template <std::size_t s, std::size_t n>
void enumerate_kmers(std::array<std::array<float, s>, n>& profile, std::string& alphabet, float T, int k,
                     std::vector<std::string>& strings, std::vector<float>& scores, std::string& new_string,
                      std::vector<float>& score_cashe, int i=0, int ik=0) {
    // This is similar to the enumeration function in the exhaustive search, however,
    // now we also check that the kmers are not part of the best string.
    for (int a = 0; a < s; a++) {
        float score = score_cashe[ik] + profile[i+ik][a];
        if (score > T) { // See comment in exhaustive search.
            new_string[i*k + ik] = alphabet[a];
            if (ik == k - 1 or i*k + ik == n-1) { // or if i*k + ik < n, for the last k-mer.
                if (not check_in_best(new_string, strings[0], i * k,i * k + k)) {
                    strings.push_back(new_string);
                    scores.push_back(score);
                }
            } else {
                score_cashe[ik + 1] = score;
                enumerate_kmers(profile, alphabet, T, k, strings, scores, new_string, score_cashe, i, (ik + 1));
            }
        }
    }
}



template <std::size_t s, std::size_t n>
auto heuristic1(std::array<std::array<float, s>, n>&  profile, std::string& alphabet, float T, int k=1){
    std::string best_string(n, ' ');
    std::vector<float> best_remaining_scores((int) n/k + 1); // (int) std::ceil((float) n/k)
    float best_score;
    get_best_string<s, n> (profile, best_remaining_scores, best_string, best_score, alphabet, k);
    std::vector<std::string> strings = {best_string};
    std::vector<float> scores = {best_score};
    for (int i = 0; i < std::ceil( (float)n/k); i++) { // For each k-sized column in the profile matrix.
        std::vector<float> score_cashe(k); score_cashe[0] = {best_remaining_scores[i]};
        std::string new_string = best_string; // The new string starts with a copy of the best string.
        enumerate_kmers(profile, alphabet, T, k, strings, scores, new_string,  score_cashe,  i, 0);
    }
    return std::make_tuple(std::move(strings), std::move(scores));
}



#endif //UNTITLED_HEURISTIC1_H
