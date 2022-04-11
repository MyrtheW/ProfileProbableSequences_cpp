#ifndef UNTITLED_EXHAUSTIVE_H
#define UNTITLED_EXHAUSTIVE_H
#include <vector>
#include <map>
#include <string>
#include <cmath>

// HELPER FUNCTIONS
// ---------------------------------------------------------------------------------------
template <std::size_t s>
std::map <char, int> create_alphabet_dict(std::string& alphabet){
    // This is a temporary function required for the funciton below
    // that turns an alphabet {'A', 'C', 'G', 'T'}; into a dictionary {'A': 0, 'C': 1, 'G': 2, 'T': 3};
    std::map <char, int>  alphabet_dict;
    for (int i = 0; i < s; i++) {
    alphabet_dict[alphabet[i]] = i;
    }
    return alphabet_dict;
}

std::map <char, int> alphabet_dict;
int nucl_to_rank(char& nucl){
    // This is a temporary function to obtain the rank of a letter in the alphabet.
    // This should later be replaced when using SeqAn alphabet.
    return alphabet_dict[nucl];
}

template <std::size_t k, std::size_t s>
std::array<std::array<float, k>,s> matrix_operation(std::array<std::array<float, k>,s>& m,
                                                    float (*func)(float)){
    // A small helper function that can be used to do operations, such as taking logarithms, on matrices of s x k in size.
    for(int i=0; i<s; i++) {
        for(int j=0; j<k; j++){
            m[i][j] = func(m[i][j]);
        }
    }
    return m;
}

// ---------------------------------------------------------------------------------------
template <std::size_t n, std::size_t s>
void enumerate_strings(std::array<std::array<float, s>, n> & profile, std::string alphabet, float T,
                        std::vector<std::string>& strings, std::vector<float>& scores,
                        int ik=0, std::array<float, n> score_cashe = {0}, std::string string = std::string(n, ' ')){
    // This enumerates the strings on demand. It starts at the first position and iterates recursively, depth-first through
    // all the letters in the alphabet. E.g. for n=3, it first creates AAA. If this string reaches the threshold,
    // it will be added to the strings vector.
    for (int a = 0; a < s; a++) {
        float score = score_cashe[ik] + profile[ik][a];
        if (score > T) {
            string[ik] = alphabet[a];
            if (ik == n - 1) {
                strings.push_back(string);
                scores.push_back(score);
            } else {
                score_cashe[ik + 1] = score;
                enumerate_strings(profile, alphabet, T, strings, scores, (ik + 1), score_cashe, string);
            }
        }
    }
}

template <std::size_t n, std::size_t s>
auto exhaustive(std::array<std::array<float, s>, n> &  profile, std::string alphabet, float T){
    // This function does the exhaustive search, and is mainly a wrapper for the recursive function that enumerates
    // the strings.
    int ik=0;
    std::vector<std::string> strings;
    std::vector<float> scores={};
    enumerate_strings(profile, alphabet,  T, strings, scores,  ik);
    return std::make_tuple(std::move(strings), std::move(scores));
    //ASK: I tried to return &strings, but that doesn't work. Now it is not so efficient, because a copy is made.
}







#endif //UNTITLED_EXHAUSTIVE_H
