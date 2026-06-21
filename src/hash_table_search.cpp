// *********************************************************
// Program: hash_table_search.cpp
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
#include <climits>
#include <iomanip>

using namespace std;

struct Node {
    long long key;
    string str;
    Node* left;
    Node* right;
    int height;
    Node(long long k, const string& s)
        : key(k), str(s), left(nullptr), right(nullptr), height(1) {}
};

static int height(Node* n) { return n ? n->height : 0; }
static int balanceFactor(Node* n) { return n ? height(n->left) - height(n->right) : 0; }
static void updateHeight(Node* n) {
    int hl = height(n->left), hr = height(n->right);
    n->height = (hl > hr ? hl : hr) + 1;
}

static Node* rotateRight(Node* y) {
    Node* x = y->left; Node* t = x->right;
    x->right = y; y->left = t;
    updateHeight(y); updateHeight(x);
    return x;
}
static Node* rotateLeft(Node* x) {
    Node* y = x->right; Node* t = y->left;
    y->left = x; x->right = t;
    updateHeight(x); updateHeight(y);
    return y;
}

static Node* insert(Node* node, long long key, const string& str) {
    if (!node) return new Node(key, str);
    if (key < node->key)      node->left = insert(node->left, key, str);
    else if (key > node->key) node->right = insert(node->right, key, str);
    else return node;

    updateHeight(node);
    int bf = balanceFactor(node);
    if (bf > 1 && key < node->left->key)  return rotateRight(node);
    if (bf < -1 && key > node->right->key) return rotateLeft(node);
    if (bf > 1 && key > node->left->key) { node->left = rotateLeft(node->left); return rotateRight(node); }
    if (bf < -1 && key < node->right->key) { node->right = rotateRight(node->right); return rotateLeft(node); }
    return node;
}

class HashTable {
public:
    explicit HashTable(size_t buckets) : table(buckets, nullptr), size(buckets) {}

    void add(long long key, const string& str) {
        size_t idx = hash(key);
        table[idx] = insert(table[idx], key, str);
    }

    // Returns true if found. Kept minimal so timing reflects the search only.
    bool search(long long target) const {
        Node* cur = table[hash(target)];
        while (cur) {
            if (target == cur->key) return true;
            cur = (target < cur->key) ? cur->left : cur->right;
        }
        return false;
    }

    // Number of key comparisons needed to locate target (its probe depth).
    int probeDepth(long long target) const {
        Node* cur = table[hash(target)];
        int depth = 0;
        while (cur) {
            ++depth;
            if (target == cur->key) return depth;
            cur = (target < cur->key) ? cur->left : cur->right;
        }
        return depth;
    }

private:
    vector<Node*> table;
    size_t size;
    size_t hash(long long key) const { return static_cast<size_t>(key % static_cast<long long>(size)); }
};

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

    vector<long long> keys;
    keys.reserve(static_cast<size_t>(n));
    HashTable ht(static_cast<size_t>(n > 0 ? n : 1));

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string keyPart, strPart;
        getline(ss, keyPart, ',');
        getline(ss, strPart, ',');
        long long key = stoll(keyPart);
        keys.push_back(key);
        ht.add(key, strPart);
    }
    in.close();

    // A single search finishes faster than the clock resolution, so every case is
    // measured the same way: a chosen key is searched REP times (cache-hot) and the
    // elapsed time is divided by REP. Using one consistent method keeps the three
    // results comparable (best <= average <= worst by comparison count).
    //  best case    -> key reached with the fewest comparisons (bucket-tree root)
    //  worst case   -> key reached with the most comparisons (deepest AVL node)
    //  average case -> mean of the per-key timings over a sample of the dataset
    long long bestKey = keys[0], worstKey = keys[0];
    int minDepth = INT_MAX, maxDepth = 0;
    for (long long i = 0; i < n; ++i) {
        int d = ht.probeDepth(keys[i]);
        if (d < minDepth) { minDepth = d; bestKey = keys[i]; }
        if (d > maxDepth) { maxDepth = d; worstKey = keys[i]; }
    }

    const long long REP = 1000000;   // repetitions per timed key
    volatile bool sink = false;      // stops the optimiser from removing the search

    auto timeKey = [&](long long key) {
        auto t0 = chrono::high_resolution_clock::now();
        for (long long r = 0; r < REP; ++r) sink = sink ^ ht.search(key);
        auto t1 = chrono::high_resolution_clock::now();
        return chrono::duration<double>(t1 - t0).count() / static_cast<double>(REP);
    };

    double best = timeKey(bestKey);
    double worst = timeKey(worstKey);

    // Average: time a sample of keys the same way and take the mean.
    long long sample = (n < 2000) ? n : 2000;
    double sumAvg = 0.0;
    for (long long i = 0; i < sample; ++i)
        sumAvg += timeKey(keys[i]);
    double average = sumAvg / static_cast<double>(sample);

    string outFile = "hash_table_search_dataset_" + to_string(n) + ".txt";
    ofstream out(outFile);
    if (!out) {
        cerr << "Error: cannot open " << outFile << '\n';
        return 1;
    }
    out << scientific << setprecision(6);
    out << "Best case time: " << best << " seconds\n";
    out << "Average case time: " << average << " seconds\n";
    out << "Worst case time: " << worst << " seconds\n";
    out.close();

    cout << scientific << setprecision(6);
    cout << "Hash table search over " << n << " searches -> " << outFile << '\n';
    cout << "Best case time: " << best << " seconds (depth " << minDepth << ")\n";
    cout << "Average case time: " << average << " seconds\n";
    cout << "Worst case time: " << worst << " seconds (depth " << maxDepth << ")\n";
    return 0;
}
