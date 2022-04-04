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
    bool operator() (const std::tuple<char, int, float>& tuple1, // string, (scores,) score
                     const std::tuple<char, int, float>& tuple2) const{
        return (std::get<2>(tuple1) > std::get<2>(tuple2)); //Or the other way around?
    }
};
struct b_best_comparator {
    bool operator() (const std::tuple<std::string, float>& tuple1, // string, (scores,) score
                     const std::tuple<std::string, float>& tuple2) const{
        return (std::get<1>(tuple1) > std::get<1>(tuple2)); //Or the other way around?
    }
};

template <std::size_t s, std::size_t n>
void enumerate_tuples(std::array<std::array<float, s>, n>& profile,
                      std::string& alphabet,
                      float T,
                      std::vector<std::tuple<char, int, float>>& tuples,
                      std::string& best_string,
                      std::array<float, (int) n +1>& best_remaining_scores){
    for (int i = 0; i < n; i++)
        for (int a = 0; a < s; a++) {
            char a_char = alphabet[a];
            if (a_char != best_string[i]) {
                float score = profile[i][a];
                if (score + best_remaining_scores[i] > T){
                    tuples.push_back(std::make_tuple(a_char, i, score));
                }
            }
        }
    std::sort(tuples.begin(), tuples.end(), tuples_comparator());
}


template <std::size_t s, std::size_t n, std::size_t k>
auto heuristic2(std::array<std::array<float, s>, n>&  profile, std::string& alphabet, float T, int b){
    std::string best_string(n, ' ');
    std::array<float, (int) n/k + 1> best_remaining_scores; // (int) std::ceil((float) n/k)
    float best_score;
    std::vector<std::tuple<char, int, float>> tuples;
    get_best_string<s, n, k> (profile, best_remaining_scores, best_string, best_score, alphabet);
    enumerate_tuples(profile, alphabet, T, tuples, best_string, best_remaining_scores);

    // Hash table
    std::unordered_map<std::string, float> strings;
    strings[best_string] = best_score;
    // Min heap
    std::vector<std::tuple<std::string, float>> b_best_strings;
    b_best_strings.push_back(std::make_tuple(best_string, best_score));

    // For loop using auto
    for (auto tup = tuples.begin(); tup != tuples.end(); tup++)
        // ASK: loop over a copy of the b_best, because of changes that are made during looping...
    {
        auto copy_b_best_strings = b_best_strings;
        for (auto existing_string_score = copy_b_best_strings.begin();
             existing_string_score != copy_b_best_strings.end(); existing_string_score++) {
            //(nucleotide, i, score) = *tup
            std::string new_string = std::get<0>(*existing_string_score); // should create a copy
            new_string[std::get<1>(*tup)] = std::get<0>(*tup);
            if (not strings[new_string]) {
                float new_score = std::get<1>(*existing_string_score)
                                  - profile[std::get<1>(*tup)][nucl_to_rank(std::get<0>(*existing_string_score)[std::get<1>(*tup)])]
                                  + std::get<2>(*tup);
                if (new_score > T) {
                    strings[new_string] = new_score;
                    if (b_best_strings.size() < b) {
                        b_best_strings.push_back(std::make_tuple(new_string, new_score));
                        std::push_heap(b_best_strings.begin(), b_best_strings.end(), b_best_comparator());
                    } else if (new_score > std::get<1>(b_best_strings[0])) {

                        std::pop_heap(b_best_strings.begin(), b_best_strings.end(), b_best_comparator());
                        b_best_strings.pop_back();

                        b_best_strings.push_back(std::make_tuple(new_string, new_score));
                        std::push_heap(b_best_strings.begin(), b_best_strings.end(), b_best_comparator());
                    }
                }
            }

        }
    }
    std::cout << &strings << "\n";
    return(&strings);        // return 2 character arrays.
}

int test_heuristic2() {
    const std::size_t n = 3;
    const std::size_t k = 1;
    const std::size_t s = 4;
    int b = 5;

    std::array<std::array<float, s>, n> profile = {{{0.3, 0.2, 0.4, 0.1,},
                                                    {0.2, 0.25, 0.25, 0.3,},
                                                    {0.5, 0.1, 0.2, 0.2,}}};
    profile = matrix_operation(profile, &std::log2f); // takes the logarithm
    std::string alphabet = {'A', 'C', 'G', 'T'};
    alphabet_dict = create_alphabet_dict<s> (alphabet);

    auto strings = heuristic2<s, n, k> (profile, alphabet, -7, b);
    std::cout << &strings << "\n";
    return 0;
}

#endif //UNTITLED_HEURISTIC2_H
