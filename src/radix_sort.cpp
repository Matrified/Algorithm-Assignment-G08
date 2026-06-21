// *********************************************************
// Program: radix_sort.cpp
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
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

struct Element {
    long long key;
    string str;
};

// LSD radix sort over the 10 digits, rightmost digit first.
static void radixSort(vector<Element>& a) {
    if (a.empty()) return;
    vector<Element> output(a.size());
    long long divisor = 1;

    for (int d = 0; d < 10; ++d) {
        int count[10] = {0};

        for (const Element& e : a)
            count[(e.key / divisor) % 10]++;

        for (int i = 1; i < 10; ++i)
            count[i] += count[i - 1];

        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            int digit = (a[i].key / divisor) % 10;
            output[--count[digit]] = a[i];
        }

        a.swap(output);
        divisor *= 10;
    }
}

int main() {
    // Input (uncomment exactly one line).
    string inputFile = "dataset_1000000.csv";
    long long n = 1000000;
    // string inputFile = "dataset_1000.csv"; long long n = 1000;

    ifstream in(inputFile);
    if (!in) {
        cerr << "Error: cannot open " << inputFile << '\n';
        return 1;
    }

    vector<Element> data;
    data.reserve(static_cast<size_t>(n));
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string keyPart, strPart;
        getline(ss, keyPart, ',');
        getline(ss, strPart, ',');
        Element e;
        e.key = stoll(keyPart);
        e.str = strPart;
        data.push_back(e);
    }
    in.close();

    // Time the sorting only (no I/O inside the measured region).
    auto begin = chrono::high_resolution_clock::now();
    radixSort(data);
    auto finish = chrono::high_resolution_clock::now();
    double seconds = chrono::duration<double>(finish - begin).count();

    string outFile = "radix_sorted_dataset_" + to_string(n) + ".csv";
    ofstream out(outFile);
    if (!out) {
        cerr << "Error: cannot open " << outFile << '\n';
        return 1;
    }
    for (const Element& e : data)
        out << e.key << '/' << e.str << '\n';
    out << "Running time: " << seconds << " seconds\n";
    out.close();

    cout << "Radix sort of " << n << " elements -> " << outFile << '\n';
    cout << "Running time: " << seconds << " seconds\n";
    return 0;
}
