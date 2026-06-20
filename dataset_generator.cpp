#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

int main() {

    // Group leader student ID seed number : 2421324366
    std::mt19937_64 srand2(2421324366ULL);

    long long n;

    std::cout << "Please enter dataset size: ";
    std::cin >> n;

    std::string filename = "dataset_" + std::to_string(n) + ".csv";
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error: unable to open file." << std::endl;
        return 1;
    }

    std::uniform_int_distribution<unsigned long long> int_dist(
        1000000000ULL, 9999999999ULL);

    std::uniform_int_distribution<int> letter_dist(0, 25);

    std::unordered_set<unsigned long long> unique_integers;

    long long generated_count = 0;

    //std::cout << "Generating " << filename << "..." << std::endl;

    while (generated_count < n) {

        unsigned long long random_id = int_dist(srand2);

        if (unique_integers.insert(random_id).second) {

            std::string random_string;

            for (int i = 0; i < 5; ++i) {
                random_string += 'a' + letter_dist(srand2);
            }

            outputFile << random_id << "," << random_string << "\n";
            generated_count++;
        }
    }

    outputFile.close();

    std::cout << "dataset generated complete: " << filename << std::endl;

    return 0;
}