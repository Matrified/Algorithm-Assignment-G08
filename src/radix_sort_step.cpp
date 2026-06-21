// *********************************************************
// Program: radix_sort_step.cpp
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

using namespace std;

struct Element {
    long long key;   // 10-digit integer
    string str;      // 5-letter string
};

// Print the array in the form: [key/str, key/str, ...] label
static void printStep(const vector<Element>& a, const string& label, ofstream& out) {
    out << '[';
    for (size_t i = 0; i < a.size(); ++i) {
        out << a[i].key << '/' << a[i].str;
        if (i + 1 < a.size()) out << ", ";
    }
    out << "] " << label << '\n';
}

// LSD radix sort on the 10-digit integer, processing the rightmost digit first.
// d = 10 is the rightmost (units) digit, d = 1 is the leftmost digit.
static void radixSortStep(vector<Element>& a, ofstream& out) {
    printStep(a, "original", out);

    long long divisor = 1;
    for (int d = 10; d >= 1; --d) {
        vector<Element> output(a.size());
        int count[10] = {0};

        for (const Element& e : a)
            count[(e.key / divisor) % 10]++;

        for (int i = 1; i < 10; ++i)
            count[i] += count[i - 1];

        // Traverse from the end to keep the sort stable.
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            int digit = (a[i].key / divisor) % 10;
            output[--count[digit]] = a[i];
        }

        a = output;
        printStep(a, "d=" + to_string(d), out);
        divisor *= 10;
    }
}

int main() {
    // Input (uncomment exactly one block).
    string inputFile = "dataset_1000.csv";
    long long n = 1000;
    int startRow = 1;
    int endRow = 7;
    // string inputFile = "dataset_1000.csv"; long long n = 1000; int startRow = 1;  int endRow = 20;

    ifstream in(inputFile);
    if (!in) {
        cerr << "Error: cannot open " << inputFile << '\n';
        return 1;
    }

    vector<Element> all;
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
        all.push_back(e);
    }
    in.close();

    if (startRow < 1) startRow = 1;
    if (endRow > static_cast<int>(all.size())) endRow = static_cast<int>(all.size());

    vector<Element> data(all.begin() + (startRow - 1), all.begin() + endRow);

    string outFile = "dataset_" + to_string(n) + "_radix_sorted_step_" +
                     to_string(startRow) + "_" + to_string(endRow) + ".txt";
    ofstream out(outFile);
    if (!out) {
        cerr << "Error: cannot open " << outFile << '\n';
        return 1;
    }

    radixSortStep(data, out);
    out.close();

    cout << "Radix sort steps written to " << outFile << '\n';
    return 0;
}
