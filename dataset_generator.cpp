//dataset generator 
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

int main() {
    // Group leader student ID seed number : 2421324366
    unsigned long long seed = 2421324366ULL; 
    std::mt19937_64 generator(seed); 

    // set dataset size 
    long long n = 1000; 
    
    std::string filename = "dataset_" + std::to_string(n) + ".csv"; 
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "error: unable to open file." << std::endl;
        return 1;
    }

    // unique integers (avoid duplicates)
    std::unordered_set<unsigned long long> unique_integers; 

    // Define 10-digit range constraints from 1,000,000,000 to 9,999,999,999
    std::uniform_int_distribution<unsigned long long> int_dist(1000000000ULL, 9999999999ULL); 
    std::uniform_int_distribution<int> letter_dist(0, 25);

    long long generated_count = 0;

    while (generated_count < n) {
        unsigned long long random_id = int_dist(generator); 

        if (unique_integers.find(random_id) == unique_integers.end()) {
            unique_integers.insert(random_id); 

            // generate the string entry
            std::string random_string = ""; 
            for (int i = 0; i < 5; ++i) {
                char random_char = 'a' + letter_dist(generator); 
                random_string += random_char;
            }

            outputFile << random_id << "," << random_string << "\n"; 
            generated_count++;
        }
    }

    outputFile.close();
    std::cout << "Dataset generation complete: " << filename << " created successfully." << std::endl;
    return 0;
}