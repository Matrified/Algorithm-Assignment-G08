# TT3L_G08 — CCP6214 Algorithm Design & Analysis (2610)

Comparative analysis of **Radix Sort** (LSD, rightmost digit first) and
**Heap Sort** (max-heap), plus a running-time analysis of **Hash Table Search**
(AVL-tree separate chaining). All algorithms are standard C++ with no
sorting/searching library functions or library data structures.

- Lecture: **TC1L** Tutorial: **TT3L** Group: **G08** Group ID: **TT3L_G08**
- Leader: Hadi Abdulla (242UC243PP) — RNG seed `2421324366`
- Members: Abazar Adam (1231302335), Hadi Abdulla (leader), Lim Chin Wei
  (253UC256V3), Nurul Asyiqin (243UC247HJ)

## Folder structure

```
TT3L_G08.docx                main report (submission document)
src/                         graded C++ deliverables
  dataset_generator.cpp        generate dataset_n.csv (unique random elements)
  radix_sort_step.cpp          radix sort, prints every digit pass
  heap_sort_step.cpp           heap sort, prints every extraction step
  radix_sort.cpp               full radix sort + running time
  heap_sort.cpp                full heap sort + running time
  hash_table_search_step.cpp   search path (direct/tree) for found & not-found
  hash_table_search.cpp        n searches, best/average/worst running times
datasets/    dataset_1000.csv (step demo), dataset_1000000.csv (full-run demo)
output/      step files, sorted files, search-time files
experiments/ experiment.cpp + experiment_results.csv (10-size radix vs heap)
README.md
```

## Build (Windows, g++ / MinGW)

```
g++ -std=c++14 -O2 -Wall src/dataset_generator.cpp     -o bin/dataset_generator.exe
g++ -std=c++14 -O2 -Wall src/radix_sort_step.cpp       -o bin/radix_sort_step.exe
g++ -std=c++14 -O2 -Wall src/heap_sort_step.cpp        -o bin/heap_sort_step.exe
g++ -std=c++14 -O2 -Wall src/radix_sort.cpp            -o bin/radix_sort.exe
g++ -std=c++14 -O2 -Wall src/heap_sort.cpp             -o bin/heap_sort.exe
g++ -std=c++14 -O2 -Wall src/hash_table_search_step.cpp -o bin/hash_table_search_step.exe
g++ -std=c++14 -O2 -Wall src/hash_table_search.cpp     -o bin/hash_table_search.exe
```

## Demo flow

Each program reads its input from the **current directory** and writes output
there. Run them from a folder that contains the dataset (copy the file from
`datasets/` next to the executables, or `cd` into a working folder). Inputs
(file name, size, start/end row, targets) are set as commented/uncommented lines
near the top of each `main()`.

| Step | Program | Input | Output |
|------|---------|-------|--------|
| 1 | `dataset_generator` | size n (in code) | `dataset_n.csv` (unsorted) |
| 2 | `radix_sort_step` | `dataset_1000.csv`, start, end | `dataset_1000_radix_sorted_step_<s>_<e>.txt` |
| 3 | `heap_sort_step` | `dataset_1000.csv`, start, end | `dataset_1000_heap_sorted_step_<s>_<e>.txt` |
| 4 | `hash_table_search_step` | `dataset_1000.csv`, found & not-found target | `dataset_1000_hash_table_search_step_<target>.txt` |
| 5 | `radix_sort` | `dataset_1000000.csv` | `radix_sorted_dataset_1000000.csv` + running time |
| 6 | `heap_sort` | `dataset_1000000.csv` | `heap_sorted_dataset_1000000.csv` + running time |
| 7 | `hash_table_search` | `dataset_1000000.csv` | `hash_table_search_dataset_1000000.txt` (best/avg/worst) |

## Notes

- Sorting key is the **10-digit integer**; the `(integer, string)` pair stays
  together in all sorted output.
- Running times exclude I/O, as required.
- `dataset_1000.csv` is the assignment sample (used by the step programs).
  `dataset_1000000.csv` is the full-run demo input. For the experiment study,
  generate 10 sizes (see `experiments/experiment.cpp`).
- The submission zip should be named `TT3L_G08.zip`. If it exceeds 99 MB, upload
  the large input/output files to a OneDrive folder and put the link in the docx.
- `bin/` (compiled executables) is git-ignored and not part of the submission.
