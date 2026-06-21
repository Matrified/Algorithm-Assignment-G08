// *********************************************************
// Program: experiment.cpp  (experiment harness, not a graded deliverable)
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC1L
// Tutorial Class: TT3L
// Trimester: 2610
// Group: TT3L_G08
// Member_1: 242UC243PP | HADI ABDULLA (leader)
// Member_2: 243UC247HJ | NURUL ASYIQIN BINTI MOHD NAZRON
// Member_3: 253UC256V3 | LIM CHIN WEI
// Member_4: 1231302335 | ABAZAR ADAM HUMED MOHAMEDALI
// Purpose: time radix sort vs heap sort over 10 input sizes, sorting only
//          (I/O excluded), and write the results to experiment_results.csv.
// *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

struct Element {
    long long key;
    string str;
};

static void radixSort(vector<Element>& a) {
    if (a.empty()) return;
    vector<Element> output(a.size());
    long long divisor = 1;
    for (int d = 0; d < 10; ++d) {
        int count[10] = {0};
        for (const Element& e : a) count[(e.key / divisor) % 10]++;
        for (int i = 1; i < 10; ++i) count[i] += count[i - 1];
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            int digit = (a[i].key / divisor) % 10;
            output[--count[digit]] = a[i];
        }
        a.swap(output);
        divisor *= 10;
    }
}

static void heapify(vector<Element>& a, int n, int i) {
    while (true) {
        int largest = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && a[l].key > a[largest].key) largest = l;
        if (r < n && a[r].key > a[largest].key) largest = r;
        if (largest == i) break;
        swap(a[i], a[largest]);
        i = largest;
    }
}

static void heapSort(vector<Element>& a) {
    int n = static_cast<int>(a.size());
    for (int i = n / 2 - 1; i >= 0; --i) heapify(a, n, i);
    for (int i = n - 1; i >= 1; --i) { swap(a[0], a[i]); heapify(a, i, 0); }
}

static vector<Element> makeDataset(long long n, mt19937_64& rng) {
    uniform_int_distribution<long long> intDist(1000000000LL, 9999999999LL);
    uniform_int_distribution<int> letterDist(0, 25);
    unordered_set<long long> seen;
    seen.reserve(static_cast<size_t>(n) * 2);
    vector<Element> data;
    data.reserve(static_cast<size_t>(n));
    while (static_cast<long long>(data.size()) < n) {
        long long v = intDist(rng);
        if (seen.insert(v).second) {
            string s(5, 'a');
            for (int i = 0; i < 5; ++i) s[i] = static_cast<char>('a' + letterDist(rng));
            data.push_back({v, s});
        }
    }
    return data;
}

int main() {
    long long sizes[] = {1000, 2000, 5000, 10000, 20000,
                         50000, 100000, 200000, 500000, 1000000};

    ofstream out("experiment_results.csv");
    out << "n,radix_seconds,heap_seconds\n";
    cout << "n\tradix(s)\theap(s)\n";

    mt19937_64 rng(2421324366ULL);   // group leader seed

    for (long long n : sizes) {
        vector<Element> base = makeDataset(n, rng);

        vector<Element> a = base;
        auto t0 = chrono::high_resolution_clock::now();
        radixSort(a);
        auto t1 = chrono::high_resolution_clock::now();
        double radixSec = chrono::duration<double>(t1 - t0).count();

        vector<Element> b = base;
        t0 = chrono::high_resolution_clock::now();
        heapSort(b);
        t1 = chrono::high_resolution_clock::now();
        double heapSec = chrono::duration<double>(t1 - t0).count();

        out << n << ',' << radixSec << ',' << heapSec << '\n';
        cout << n << '\t' << radixSec << '\t' << heapSec << '\n';
    }

    out.close();
    return 0;
}
