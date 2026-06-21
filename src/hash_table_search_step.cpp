// *********************************************************
// Program: hash_table_search_step.cpp
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
// Member_1 (Hadi Abdulla): Radix sort (step + full), experiment runs, documentation
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

// Each hash bucket holds an AVL balanced binary search tree keyed on the integer.
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
    Node* x = y->left;
    Node* t = x->right;
    x->right = y;
    y->left = t;
    updateHeight(y);
    updateHeight(x);
    return x;
}

static Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* t = y->left;
    y->left = x;
    x->right = t;
    updateHeight(x);
    updateHeight(y);
    return y;
}

static Node* insert(Node* node, long long key, const string& str) {
    if (!node) return new Node(key, str);
    if (key < node->key)      node->left = insert(node->left, key, str);
    else if (key > node->key) node->right = insert(node->right, key, str);
    else return node;                       // unique keys, ignore duplicate

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

    // Search and record the path. Returns the matching node or nullptr.
    Node* searchStep(long long target, ofstream& out) {
        size_t idx = hash(target);
        Node* cur = table[idx];

        out << "bucket: " << idx << '\n';
        if (!cur) {
            out << "path: empty bucket\n";
            return nullptr;
        }

        // "direct" if the target sits at the bucket's tree root; otherwise "tree".
        bool atRoot = (cur->key == target);
        out << "path (" << (atRoot ? "direct" : "tree") << "): ";

        Node* found = nullptr;
        bool first = true;
        while (cur) {
            out << (first ? "" : " -> ") << cur->key;
            first = false;
            if (target == cur->key) { found = cur; break; }
            cur = (target < cur->key) ? cur->left : cur->right;
        }
        out << '\n';
        return found;
    }

    size_t bucketCount() const { return size; }

private:
    vector<Node*> table;
    size_t size;
    size_t hash(long long key) const { return static_cast<size_t>(key % static_cast<long long>(size)); }
};

static size_t pickBucketCount(long long n) {
    // A prime-ish count near n keeps the load factor close to 1.
    long long m = n > 0 ? n : 1;
    return static_cast<size_t>(m);
}

int main() {
    // Input (uncomment exactly one block).
    string inputFile = "dataset_1000.csv";
long long n = 1000;

// set found target explicitly as in the sample
long long foundTarget = 8937497183;  // key from the first row of dataset_1000.csv
long long notFoundTarget = 123456789;  // 9-digit value can never exist in a 10-digit set

    ifstream in(inputFile);
    if (!in) {
        cerr << "Error: cannot open " << inputFile << '\n';
        return 1;
    }

    HashTable ht(pickBucketCount(n));
    long long firstKey = -1;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string keyPart, strPart;
        getline(ss, keyPart, ',');
        getline(ss, strPart, ',');
        long long key = stoll(keyPart);
        if (firstKey < 0) firstKey = key;
        ht.add(key, strPart);
    }
    in.close();

    if (foundTarget == 0) foundTarget = firstKey;

    // Found target output.
    {
        string outFile = "dataset_" + to_string(n) +
                         "_hash_table_search_step_" + to_string(foundTarget) + ".txt";
        ofstream out(outFile);
        Node* hit = ht.searchStep(foundTarget, out);
        if (hit) out << foundTarget << " = " << hit->key << '/' << hit->str << '\n';
        else     out << "-1 != " << foundTarget << '\n';
        out.close();
        cout << "Search step (found) -> " << outFile << '\n';
    }

    // Not-found target output.
    {
        string outFile = "dataset_" + to_string(n) +
                         "_hash_table_search_step_" + to_string(notFoundTarget) + ".txt";
        ofstream out(outFile);
        Node* hit = ht.searchStep(notFoundTarget, out);
        if (hit) out << notFoundTarget << " = " << hit->key << '/' << hit->str << '\n';
        else     out << "-1 != " << notFoundTarget << '\n';
        out.close();
        cout << "Search step (not found) -> " << outFile << '\n';
    }

    return 0;
}
