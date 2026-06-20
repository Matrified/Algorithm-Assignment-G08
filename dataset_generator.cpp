#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <unordered_set>

int main() {

    // seed from student ID
    srand(2421324366);

    long long sizes[10];

    // generate 10 dataset sizes
    for (int i = 0; i < 10; ++i) {
        sizes[i] = 1000 + (rand() % 990001); // 1000 to 1,000,000
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

            // 10-digit number range
            unsigned long long random_id =
                1000000000ULL + (rand() % 9000000000ULL);

            if (unique_integers.insert(random_id).second) {

                std::string random_string;

                for (int j = 0; j < 5; ++j) {
                    random_string += 'a' + (rand() % 26);
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