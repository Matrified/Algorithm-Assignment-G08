// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC1L
// Tutorial Class: TT3L
// Trimester: 2610
// Member_1: 242UC243PP | HADI ABDULLA | hadiabdullakannanchery@gmail.com | +6019847432 (leader)
// Member_2: 243UC247HJ | NURUL ASYIQIN BINTI MOHD NAZRON | nurul.asyiqin.mohd@student.mmu.edu.my | +60196911531
// Member_3: 253UC256V3 | LIM CHIN WEI | cwlim991@gmail.com | +60163504810
// Member_4: 1231302335 | ABAZAR ADAM HUMED MOHAMEDALI | abazaradamwork@gmail.com | +60175803025
// *********************************************************
// Task Distribution
// Member_1 (Hadi Abdulla): Dataset generator, experiment runs, documentation
// Member_2 (Nurul Asyiqin): Radix sort (step + full), experiment runs, documentation
// Member_3 (Lim Chin Wei): Heap sort (step + full), experiment runs, documentation
// Member_4 (Abazar Adam): Hash table search (step + full), experiment runs, documentation
// *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <string>

using namespace std;

int main() {
    // Seed = group leader student ID 242UC243PP mapped to digits (2421324366).
    mt19937_64 rng(2421324366ULL);

    // Input: one dataset size n. Uncomment exactly one line.
    long long n = 1000;
    // long long n = 10000;
    // long long n = 100000;
    // long long n = 1000000;
    // long long n = 10000000;

    // 10-digit positive integers: 1,000,000,000 .. 9,999,999,999
    const long long LOW  = 1000000000LL;
    const long long HIGH = 9999999999LL;

    uniform_int_distribution<long long> intDist(LOW, HIGH);
    uniform_int_distribution<int> letterDist(0, 25);

    string filename = "dataset_" + to_string(n) + ".csv";
    ofstream out(filename);
    if (!out) {
        cerr << "Error: cannot open " << filename << " for writing.\n";
        return 1;
    }

    unordered_set<long long> seen;
    seen.reserve(static_cast<size_t>(n) * 2);

    long long count = 0;
    while (count < n) {
        long long value = intDist(rng);
        if (seen.insert(value).second) {           // keep only unique integers
            string s(5, 'a');
            for (int i = 0; i < 5; ++i)
                s[i] = static_cast<char>('a' + letterDist(rng));
            out << value << ',' << s << '\n';
            ++count;
        }
    }

    out.close();
    cout << "Generated " << n << " unique random elements -> " << filename << '\n';
    return 0;
}
