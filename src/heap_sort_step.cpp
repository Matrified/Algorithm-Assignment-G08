// *********************************************************
// Program: heap_sort_step.cpp
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
/// Member_1 (Hadi Abdulla): Radix sort (step + full), experiment runs, documentation
// Member_2 (Nurul Asyiqin): Dataset generator, experiment runs, documentation
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
    long long key;
    string str;
};

static void printStep(const vector<Element>& a, const string& label, ofstream& out) {
    out << '[';
    for (size_t i = 0; i < a.size(); ++i) {
        out << a[i].key << '/' << a[i].str;
        if (i + 1 < a.size()) out << ", ";
    }
    out << "] " << label << '\n';
}

// Sift down node i within a heap of size n (max-heap on the integer key).
static void heapify(vector<Element>& a, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left].key > a[largest].key) largest = left;
    if (right < n && a[right].key > a[largest].key) largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        heapify(a, n, largest);
    }
}

static void heapSortStep(vector<Element>& a, ofstream& out) {
    int n = static_cast<int>(a.size());

    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(a, n, i);
    printStep(a, "initial", out);

    for (int i = n - 1; i >= 1; --i) {
        swap(a[0], a[i]);          // move current max to the end
        heapify(a, i, 0);          // restore heap on the reduced range
        printStep(a, "i = " + to_string(i), out);
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

    string outFile = "dataset_" + to_string(n) + "_heap_sorted_step_" +
                     to_string(startRow) + "_" + to_string(endRow) + ".txt";
    ofstream out(outFile);
    if (!out) {
        cerr << "Error: cannot open " << outFile << '\n';
        return 1;
    }

    heapSortStep(data, out);
    out.close();

    cout << "Heap sort steps written to " << outFile << '\n';
    return 0;
}
