#ifndef UNTITLED_HEURISTIC2_H
#define UNTITLED_HEURISTIC2_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <queue>


struct tuples_comparator {
    bool operator() (const std::tuple<char, int, float>& tuple1,
                     const std::tuple<char, int, float>& tuple2) const{
        return (std::get<2>(tuple1) > std::get<2>(tuple2));
    }
};
struct b_best_comparator {
    bool operator() (const std::tuple<std::string, float>& tuple1,
                     const std::tuple<std::string, float>& tuple2) const{
        return (std::get<1>(tuple1) > std::get<1>(tuple2));
    }
};


template <std::size_t s, std::size_t n>
void enumerate_tuples(std::array<std::array<float, s>, n>& profile,
                      std::string& alphabet,
                      float T,
                      std::vector<std::tuple<char, int, float>>& tuples,
                      std::string& best_string,
                      std::vector<float> best_remaining_scores, float best_score){
    for (int i = 0; i < n; i++)
        for (int a = 0; a < s; a++) {
            char a_char = alphabet[a];
            if (a_char != best_string[i]) {
                float score = profile[i][a];
                if (score + best_score > T){ //CHANGE
                    tuples.push_back(std::make_tuple(a_char, i, score));
                }
            }
        }
    std::sort(tuples.begin(), tuples.end(), tuples_comparator());
}

template <std::size_t s, std::size_t n>
void get_best_string_old(std::array<std::array<float, s>, n>& profile,
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
            float max_score = *max;
            score += *max;
            best_string[i*k + ik] = alphabet[std::distance(profile_column.begin(), max)];
            // The best scoring k-mers are used to generate the best overall string.
            for (int a = 0; a < s; a++){ profile_column[a] -= max_score;} //CHANGE
        }
        best_kmer_scores[i] = score;
    }
    best_score = std::accumulate(best_kmer_scores.begin(), best_kmer_scores.end(), 0.0);
    for (int i = 0; i < std::ceil( n/k); i++) {
        best_remaining_scores[i] = best_score - best_kmer_scores[i];
    }
}

template <std::size_t s, std::size_t n>
auto heuristic2(std::array<std::array<float, s>, n>  profile, std::string& alphabet, float T, int b){
    // Initialize datastructures and add the best string to them.
    std::string best_string(n, ' ');
    std::vector<float> best_remaining_scores(n);
    float best_score;
    std::vector<std::tuple<char, int, float>> tuples;
    get_best_string_old(profile, best_remaining_scores, best_string, best_score, alphabet);
    enumerate_tuples(profile, alphabet, T, tuples, best_string, best_remaining_scores, best_score);

    std::unordered_map<std::string, float> strings; // Hash table
    strings[best_string] = best_score;

    std::vector<std::tuple<std::string, float>> b_best_strings; // Min heap containing the b best strings
    b_best_strings.push_back(std::make_tuple(best_string, best_score));

    // Loop over the tuples of nucleotides, and add purmutate the b best strings at these positions.
    for (auto tup = tuples.begin(); tup != tuples.end(); tup++) // Each tuple consists of (nucleotide, i, score)
        // I loop over a copy of the b_best, because of changes that are made during looping.
    {
        auto copy_b_best_strings = b_best_strings;
        for (auto existing_string_score = copy_b_best_strings.begin();
            existing_string_score != copy_b_best_strings.end(); existing_string_score++) {
            int i = std::get<1>(*tup);
            float new_score = std::get<1>(*existing_string_score)
                              - profile[i][nucl_to_rank(std::get<0>(*existing_string_score)[i])]
                              + std::get<2>(*tup); // calculate the new score from the existing string and the value of the nucleotide that is going to be replaced.
            if (new_score > T) {
                std::string new_string = std::get<0>(*existing_string_score);
                new_string[std::get<1>(*tup)] = std::get<0>(*tup);
                if (not strings[new_string]){ //strings.find(new_string) == strings.end()) { // Check if element already exists in the hash table. You  must explicitely do this, otherwise you might insert duplicates in the top b.
                    // Alternatively you can dot his before calculating and thresholding the score.
                    strings[new_string] = new_score;
                    if (b_best_strings.size() < b) {
                        //If the b best strings is not completely filled yet, then the new string needs to be added:
                        b_best_strings.push_back(std::make_tuple(std::move(new_string), std::move(new_score)));
                        std::push_heap(b_best_strings.begin(), b_best_strings.end(), b_best_comparator());
                    } else if (new_score > std::get<1>(b_best_strings[0])) {
                        //If the new string has a high enough score, it needs to be placed in the b best strings:
                        std::pop_heap(b_best_strings.begin(), b_best_strings.end(), b_best_comparator());
                        b_best_strings.pop_back();

                        b_best_strings.push_back(std::make_tuple(std::move(new_string), std::move(new_score)));
                        std::push_heap(b_best_strings.begin(), b_best_strings.end(), b_best_comparator());
                    }
                }
            }
        }
    }
    return(strings);
}



#endif //UNTITLED_HEURISTIC2_H
