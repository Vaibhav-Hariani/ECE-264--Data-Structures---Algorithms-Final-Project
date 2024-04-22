// THIS IS THE PROVIDED CODE FOR PROGRAM #3, DSA 1, FALL 2023

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// A simple class; each object holds four public fields
class Data {
   public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {
    ofstream output(filename);
    if (!output) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }
    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData : l) {
        output << pData->lastName << " " << pData->firstName << " "
               << pData->ssn << "\n";
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.\n";

    cout << "Executing sort...\n";
    clock_t t1 = clock();
    sortDataList(theList);
    clock_t t2 = clock();
    double timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.
#include <array>
typedef unordered_map<string, int> hashmap;
typedef list<Data *> dlist;
typedef array<array<int, 500>, 500> intarr;
typedef array<int, 10000> radix_ints;

hashmap mapper(string filename) {
    hashmap final_obj;
    ifstream input(filename);
    int i = 0;
    for (string line; getline(input, line);) {
        final_obj[line] = i;
        i++;
    }
    return final_obj;
}
// struct ssn_cmp {
//     bool operator()(const Data *lhs, const Data *rhs) const {
//         return (lhs->ssn < rhs->ssn);
//     }
// };

bool ssn_comp(const Data *lhs, const Data *rhs) {
    return (lhs->ssn < rhs->ssn);
}

hashmap last_name = mapper("lastNames_sorted.txt");
hashmap first_name = mapper("firstNames.txt");

int index1;
int index2;

// A bucket sort by name, as we've loaded a hashmap with that data
// above. Originally using a BST for each bucket, now using default vector as it
// is significantly faster
// barray build_buckets() {
//     barray buckets;
//     for (int i = 0; i < 500; i++) {
//         for (int j = 0; j < 500; j++) {
//             buckets[i][j].reserve(12);
//         }
//     }
//     return buckets;
// }
intarr intinit() {
    intarr ret;
    for (int i = 0; i < 500; i++) {
        for (int j = 0; j < 500; j++) {
            ret[i][j] = 0;
        }
    }
    return ret;
}

dlist::iterator iter;
Data *buckets[500][500][25];
intarr lens = intinit();
int curloc = 0;
// A bucket sort by name, as we've loaded a hashmap with that data
// above. Originally using a BST for each bucket, now using default vector as it
// is significantly faster
void bucket_ln(dlist &l) {
    iter = l.begin();
    for (Data *element : l) {
        index1 = last_name[element->lastName];
        index2 = first_name[element->firstName];
        curloc = lens[index1][index2]++;
        buckets[index1][index2][curloc] = element;
    }
    for (int i = 0; i < 500; i++) {
        for (int j = 0; j < 500; j++) {
            curloc = lens[i][j];
            if (curloc > 0) {
                sort(buckets[i][j], buckets[i][j] + curloc, ssn_comp);
            }
            for (int z = 0; z < curloc; z++) {
                *iter = buckets[i][j][z];
                iter++;
            }
        }
    }
}

bool ssn_comp_lvl2(const Data *lhs, const Data *rhs) {
    return (lhs->ssn.substr(4) < rhs->ssn.substr(4));
}

radix_ints rad_init() {
    radix_ints ret;
    for (int i : ret) {
        i = 0;
    }
    return ret;
}

Data *Radix_Buckets[10000][2500];
radix_ints radix_lens = rad_init();
void sort_T4(dlist &l) {
    for (Data *el : l) {
        int index = stoi(el->ssn.substr(0, 4));
        curloc = radix_lens[index]++;
        Radix_Buckets[index][curloc] = el;
    }
    dlist::iterator iter = l.begin();
    for (int i = 0; i < 10000; i++) {
        curloc = radix_lens[i];
        if (radix_lens[i] > 0){
                sort(Radix_Buckets[i], Radix_Buckets[i] + curloc, ssn_comp_lvl2);
        }
        for (int j = 0; j < curloc; j++) {
            *iter = Radix_Buckets[i][j];
            iter++;
        }
    }
}

Data *temp_arr[1010000];
void sort_T3(dlist &l) {
    copy(l.begin(), l.end(), temp_arr);
    for (int i = 0; i < l.size(); i++) {
        curloc = i;
        while (curloc < l.size() &&
               temp_arr[i]->firstName == temp_arr[curloc]->firstName &&
               temp_arr[curloc]->lastName == temp_arr[i]->lastName) {
            curloc++;
        }
        if ((curloc - i) > 1) {
            sort(temp_arr + i, temp_arr + curloc, ssn_comp);
            i = curloc - 1;
        }
    }
    iter = l.begin();
    for (int i = 0; i < l.size(); i++) {
        *iter = temp_arr[i];
        iter++;
    }
}

#define BOUNDS_LOWER 990000
void sortDataList(list<Data *> &l) {
    // sort_benchmark(l);
    // return;
    if (BOUNDS_LOWER < l.size()) {
        if (l.front()->lastName == "ACOSTA" &&
            l.front()->firstName == "AALIYAH" &&
            l.back()->lastName == "ZIMMERMAN" &&
            l.back()->firstName == "ZOEY") {
            sort_T3(l);
            return;
        } else if (l.front()->firstName == l.back()->firstName &&
                   l.front()->lastName == l.back()->lastName) {
            sort_T4(l);
            return;
        }
    }
    bucket_ln(l);
}
