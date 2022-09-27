#include <iostream>
#include "exhaustive.h"
#include "heuristic1.h"
#include "heuristic2.h"
#include "heuristic3.h"
#include "evaluation .h"
#include "test_example.h"
//#include seqan

int main() {
//    std::string folder= "C:\\Users\\myrth\\Documents\\SCHOOL\\1E SEMESTER MASTER\\S; stage\\Python code\\data\\RNA_profiles\\"; std::size_t s =  4;
//    std::vector<std::string> file_names = {"RF00015.afa.txt.txt", "RF00523.afa.txt.txt", "RF00005.afa.txt.txt"};
    std::string folder= "C:\\Users\\myrth\\Documents\\SCHOOL\\1E SEMESTER MASTER\\S; stage\\Python code\\data\\DNA_profiles\\"; std::size_t s =  4;
    std::vector<std::string> file_names = {"MA0007.1.jaspar.txt", "MA0528.1.jaspar.txt", "MA0659.1.jaspar.txt"};
//    std::string folder= "C:\\Users\\myrth\\Documents\\SCHOOL\\1E SEMESTER MASTER\\S; stage\\Python code\\data\\protein_profiles\\"; std::size_t s =  21;
//    std::vector<std::string> file_names = { "PS51017.fasta.fasta.txt", "PS51801.fasta.fasta.txt","PS50802.fasta.fasta.txt",};
    std::size_t n =  20;

    float T_perc=0.05;
    for (auto file_name: file_names ) {
        evaluation_top_fraction(folder, file_name, n, s, T_perc);
        evaluation_vsn(folder, file_name, 20, T_perc, s);
        distribution(folder, file_name, T_perc,n, s);
    }
    return 0;
}
