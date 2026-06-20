#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

int main() {

    // Group leader student ID seed number : 2421324366
    unsigned long long seed = 2421324366ULL;
    std::mt19937_64 generator(seed);

    std::uniform_int_distribution<long long> size_dist(1000, 1000000);
    std::uniform_int_distribution<unsigned long long> int_dist(1000000000ULL, 9999999999ULL);
    std::uniform_int_distribution<int> letter_dist(0, 25);

    long long sizes[10];

    for (int i = 0; i < 10; ++i) {
        sizes[i] = size_dist(generator);
    }

    // generate datasets
    for (int i = 0; i < 10; ++i) {

        long long n = sizes[i];

        std::string filename = "dataset_" + std::to_string(n) + ".csv";
        std::ofstream outputFile(filename);

        if (!outputFile.is_open()) {
            std::cerr << "error opening file" << std::endl;
            return 1;
        }

        std::unordered_set<unsigned long long> unique_integers;
        long long generated_count = 0;

        while (generated_count < n) {

            unsigned long long random_id = int_dist(generator);

            if (unique_integers.insert(random_id).second) {

                std::string random_string;
                for (int j = 0; j < 5; ++j) {
                    random_string += 'a' + letter_dist(generator);
                }

                outputFile << random_id << "," << random_string << "\n";
                generated_count++;
            }
        }

        outputFile.close();
        std::cout << "Dataset generated complete: " << filename << std::endl;
    }

    return 0;
}