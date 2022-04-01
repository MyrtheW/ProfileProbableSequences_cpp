#ifndef UNTITLED_EXHAUSTIVE_H
#define UNTITLED_EXHAUSTIVE_H
#include <vector>
#include <map>
#include <string>
#include <cmath>
// https://sgssgene-implementingsea-xsaozq1kabb.ws-eu38.gitpod.io/
// also look at code of Jörg: https://github.com/seqan/mars/blob/9db7deee6e23be3e2af8508cb5cb1b351c028556/src/profile_char.hpp


// ASK: how to efficiently store and use a (fixed size) matrix in c++. Vector of vector is easy, but has problems, but I cant find a better way yet
// ASK: should I use floats or doubles?
// TODO:  change the character arrays to strings. , e.g. the "icashe" (change that to "new_string") anyway...

// Helper functions:
// ---------------------------------------------------------------------------------------
// some printing functions:
template <typename T>
void print(T a) {
    std::cout << a << ' ';
}
template <typename T, std::size_t k>
void print(std::array <T,k>  const &a) {
    std::cout << '[';
    for(int i=0; i < k; i++)
        print(a.at(i));
    std::cout << ']';
}
template <typename T>
void print(std::vector <T> const &a) {
    std::cout << '{';
    for(int i=0; i < a.size(); i++)
        print(a.at(i));
    std::cout << '}';
}
template <std::size_t s>
std::map <char, int> create_alphabet_dict(std::string& alphabet){
    //char alphabet[s] = {'A', 'C', 'G', 'T'}; //= {'A': 0, 'C': 1, 'G': 2, 'T': 3};
    std::map <char, int>  alphabet_dict;
    for (int i = 0; i < s; i++) {
    alphabet_dict[alphabet[i]] = i;
    }
    return alphabet_dict;
        }
std::map <char, int> alphabet_dict;

int nucl_to_rank(char& nucl){
    return alphabet_dict[nucl];
}

template <std::size_t k, std::size_t s>
double profile_score(std::array<std::array<float, k>,s> profile, char *kmer){
    // assert whether length of the kmer == length profile
    double score = 0;
    for (int i = 0; i < k; i++) {
        score += profile[nucl_to_rank(kmer[i])][i];
    }
    return score;
}

template <std::size_t k, std::size_t s>
std::array<std::array<float, k>,s> matrix_operation(std::array<std::array<float, k>,s>& m,
                                                    float (*func)(float)){
    for(int i=0; i<s; i++) {
        for(int j=0; j<k; j++){
            m[i][j] = func(m[i][j]);
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return m;
}

// ---------------------------------------------------------------------------------------
template <std::size_t k, std::size_t s>
void enumerate_kmers(std::array<std::array<float, k>,s>& profile, std::string alphabet, float T,
                        std::vector<std::array<char, k>>& strings, std::vector<float>& scores,
                        int ik=0, std::array<float, k> cashe={0}, std::array<char, k> icashe={}){
    for (int a = 0; a < s; a++) {
        float score = cashe[ik] + profile[ik][a];
        if (score > T) {
            if (ik == k - 1) {
                icashe[ik] = alphabet[a];
                strings.push_back(icashe);
                scores.push_back(score);
            } else {
                cashe[ik + 1] = score;
                icashe[ik] = alphabet[a];
                enumerate_kmers(profile, alphabet, T, strings, scores, (ik + 1), cashe, icashe);
            }
        }
    }
} // the function does not output anything, because it changes the input-datastructures.


template <std::size_t k, std::size_t s>
int exhaustive(std::array<std::array<float, k>,s>&  profile, std::string alphabet, float T){
    int ik=0;
    std::array<float, k> cashe = {0}; // float cashe[k]={0};
    std::array<char, k> icashe = {}; // char icashe[k]={};
    std::vector<std::array<char, k>> strings; //[]={}
    std::vector<float> scores={};
    enumerate_kmers(profile, alphabet,  T, strings, scores,  ik, cashe, icashe);
    print(scores);
    std::cout << std::endl;
    print(strings);
    return 0;        // return 2 character arrays.
}

int test_exhaustive() {
    const std::size_t k = 3;
    const std::size_t s = 4;
    std::array<std::array<float, k>,s> profile = {{{0.3, 0.2, 0.5, },
                                                  {0.2, 0.25, 0.1, },
                                                  {0.4, 0.25, 0.2,  },
                                                  {0.1, 0.3, 0.2, }}};
    //TODO: ASK: maybe create a struct of the profile that also contains the alphabet.
    // Maybe it is easier to store the matrix transposed, because than it is easier to slice it for shorter k-mers?

    profile = matrix_operation(profile, &std::log2f); // takes the logarithm
    std::string alphabet = {'A', 'C', 'G', 'T'};
    alphabet_dict = create_alphabet_dict<s> (alphabet);
    char kmer[k] = {'A', 'A', 'A'};
    double score = profile_score(profile, kmer);
    exhaustive(profile, alphabet, -7);
    return 0;
}





#endif //UNTITLED_EXHAUSTIVE_H
