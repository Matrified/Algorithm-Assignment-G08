#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <limits>

using namespace std;

// Data structure matching your group's generator types
struct Record {
    unsigned long long key; // Uses unsigned long long for 10-digit integer IDs
    string value;           // Uses string for the 5-letter lowercase value
};

// ===== UTILITY FUNCTIONS FROM LEADER'S TEMPLATE =====

void printHeader(string title) {
    cout << "========================================\n";
    cout << "        " << title << "\n";
    cout << "========================================\n";
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidChoice(int min, int max) {
    int choice;
    while (true) {
        cout << "Enter choice: ";
        if (cin >> choice) {
            if (choice >= min && choice <= max) {
                return choice;
            } else {
                cout << "Error: Please enter a number between " << min << " and " << max << ".\n";
            }
        } else {
            cout << "Error: Invalid input. Please enter a number.\n";
            clearInputBuffer();
        }
    }
}

char getValidYesNo() {
    char response;
    while (true) {
        cout << "\nPerform another operation? (y/n): ";
        if (cin >> response) {
            if (response == 'y' || response == 'Y' || response == 'n' || response == 'N') {
                return response;
            } else {
                cout << "Error: Please enter 'y' for yes or 'n' for no.\n";
            }
        } else {
            cout << "Error: Invalid input. Please enter 'y' or 'n'.\n";
            clearInputBuffer();
        }
    }
}

// ===== ALGORITHM IMPLEMENTATION =====

// Standard Heapify function to maintain Max-Heap properties
void heapify(vector<Record>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].key > arr[largest].key)
        largest = left;

    if (right < n && arr[right].key > arr[largest].key)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Core Heap Sort Implementation
void heapSort(vector<Record>& arr) {
    int n = arr.size();

    // Step 1: Build the Max-Heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Step 2: Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]); 
        heapify(arr, i, 0);   
    }
}

// Step Tracer Function to track array snapshots across iterations
void heapSortStep(vector<Record>& arr, int startRow, int endRow, ofstream& stepFile) {
    int sz = arr.size();

    // 1. Build the initial Max-Heap structure bottom-up
    for (int i = sz / 2 - 1; i >= 0; i--) {
        heapify(arr, sz, i);
    }
    
    // Print the initial snapshot state
    stepFile << "[";
    for (long long i = startRow; i <= endRow && i < arr.size(); i++) {
        stepFile << arr[i].key << "/" << arr[i].value << (i == endRow ? "" : ", ");
    }
    stepFile << "] initial\n";

    // 2. Trace the required extraction iterations (i values)
    int trackedSteps = 1;
    for (int i = sz - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
        
        // Tracks exact steps up to 6 loops to match assignment evaluation sample requirements
        if (trackedSteps <= 6) {
            stepFile << "[";
            for (long long j = startRow; j <= endRow && j < arr.size(); j++) {
                stepFile << arr[j].key << "/" << arr[j].value << (j == endRow ? "" : ", ");
            }
            stepFile << "] i=" << i << "\n";
            trackedSteps++;
        }
    }
}

int main() {
    printHeader("Heap Sort Performance System");
    
    char loopChoice = 'y';
    while (loopChoice == 'y' || loopChoice == 'Y') {
        
        cout << "\nSelect Operation Mode:\n";
        cout << "1. Run Performance Benchmark (Full Sort)\n";
        cout << "2. Run Step-by-Step Tracer (Generate Step File)\n";
        cout << "0. Exit Program\n";
        cout << "----------------------------------------\n";
        
        int mode = getValidChoice(0, 2);
        if (mode == 0) {
            break;
        }

        long long n;
        cout << "Enter target dataset size: ";
        if (!(cin >> n)) {
            cout << "Invalid size entry.\n";
            clearInputBuffer();
            continue;
        }

        string inputFilename = "dataset_" + to_string(n) + ".csv";
        ifstream inFile(inputFilename);
        if (!inFile) {
            cout << "Error: " << inputFilename << " not found! Run dataset_generator.cpp first.\n";
            continue;
        }

        vector<Record> arr;
        string line;
        while (getline(inFile, line)) {
            size_t comma = line.find(',');
            if (comma != string::npos) {
                Record r;
                r.key = stoull(line.substr(0, comma)); 
                r.value = line.substr(comma + 1);      
                arr.push_back(r);
            }
        }
        inFile.close();

        if (mode == 1) {
            cout << "\nSorting dataset of size " << n << " using Heap Sort...\n";
            
            auto start = chrono::high_resolution_clock::now();
            heapSort(arr);
            auto end = chrono::high_resolution_clock::now();
            
            chrono::duration<double> elapsed = end - start;
            
            cout << "\n----------------------------------------\n";
            cout << "        BENCHMARK RESULTS\n";
            cout << "----------------------------------------\n";
            cout << "Execution Time: " << elapsed.count() << " seconds\n";
            cout << "----------------------------------------\n";

            string outputFilename = "heap_sort_dataset_" + to_string(n) + ".csv";
            ofstream outFile(outputFilename);
            for (const auto& r : arr) {
                outFile << r.key << "," << r.value << "\n";
            }
            outFile.close();
            cout << "Sorted data successfully written to: " << outputFilename << "\n";

        } else if (mode == 2) {
            long long startRow, endRow;
            cout << "Enter start row for tracking: ";
            cin >> startRow;
            cout << "Enter end row for tracking: ";
            cin >> endRow;

            string stepFilename = "dataset_" + to_string(n) + "_heap_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";
            ofstream stepFile(stepFilename);
            
            // Call the newly packaged step tracer function cleanly
            heapSortStep(arr, startRow, endRow, stepFile);

            stepFile.close();
            cout << "\nStep tracer text file successfully written: " << stepFilename << "\n";
        }

        loopChoice = getValidYesNo();
    }

    cout << "\n========================================\n";
    cout << "        Program Ended. Goodbye!\n";
    cout << "========================================\n";
    return 0;
}
