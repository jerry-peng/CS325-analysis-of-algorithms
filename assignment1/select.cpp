#include <iostream>
#include <vector>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <cstring>
using namespace std;

unsigned int kthSmallestSearch(ifstream*, int, int, int);
unsigned int search(ifstream*, int, int, int, vector<int>&, vector<int>&, vector<int>&, int);
int binarySearch(ifstream*, int, int, int, unsigned int);
void outputToFile(unsigned int);
void printFile(ifstream*, int, int, int);
void printVar(ifstream*, int, int, vector<int>, vector<int>, vector<int>, int);
vector<int> getFileInfo();
string intToString(int);
unsigned int hexToInt(string);
unsigned int seek(ifstream&, int);

int main() {
    // initialize variables
    vector<int> input = getFileInfo();
    int m = input[0];
    int n = input[1];
    int k = input[2];
    ifstream dataFiles[10];
    unsigned int result;

    // get input file streams
    for (int i = 0; i < m; i++) {
        string fileName = intToString(i + 1) + ".dat";
        dataFiles[i].open(fileName.c_str());
    }

    // Debug to show all numbers in the files
    //printFile(dataFiles, m, n, k);

    // run the algorithm
    result = kthSmallestSearch(dataFiles, m, n, k);

    cout << result << endl;
    // Output result to output.txt
    //outputToFile(result);

    // close all files that are opened
    for (int i = 0; i < m; i++) {
        dataFiles[i].close();
    }
}

/* Main algorithm function */
unsigned int kthSmallestSearch(ifstream* files, int m, int n, int k) {
    vector<int> min(m, -1);
    vector<int> max(m, n);
    vector<int> index(m, (n - 1)/2);
    return search(files, m, n, k, min, max, index, 0);
}

/* recursive file search function */
unsigned int search(ifstream* files, int m, int n, int k, \
        vector<int> &min, vector<int> &max, vector<int> &index, int fileIdx) {

    unsigned int val;
    int high, low;

    // Get the new index on current file,
    // get value and binary search thru rest of files
    index[fileIdx] = (max[fileIdx] + min[fileIdx]) / 2;
    val = seek(files[fileIdx], index[fileIdx]);

    for (int i = fileIdx + 1; i < m; i++) {
        high = max[i];
        low = min[i];
        index[i] = binarySearch(files, i, high, low, val);
    }

    // Calculate the total index
    int total = m;
    for (int i = 0; i < m; i++) {
        total += index[i];
    }

    // If total is high, move max, otherwise, move min
    if (total < k) {
        for (int i = fileIdx; i < m; i++) {
            min[i] = index[i];
        }
    }
    if (total > k){
        max[fileIdx] = index[fileIdx];
        for(int i = fileIdx + 1; i < m; i++) {
            max[i] = index[i] + 1;
        }
    }

    // If the index add up to k, return the number
    if (total == k) {
        // Debug to show all variables
        //printVar(files, fileIdx, total, min, max, index, m);

        return seek(files[fileIdx], index[fileIdx]);
    }
    // If the min and max are adjacent, increment file index
    if (min[fileIdx] == max[fileIdx] - 1) {
        // Debug to show all variables
        //printVar(files, fileIdx, total, min, max, index, m);

        index[fileIdx] = min[fileIdx];
        fileIdx++;
    }

    // recursive call
    return search(files, m, n, k, min, max, index, fileIdx);
}

/* binary search finction */
int binarySearch(ifstream* files, int fileIdx, int high, int low, unsigned int val) {
    if (low == high - 1) {
        return low;
    }

    int mid = (high + low) / 2;
    unsigned int seekedVal = seek(files[fileIdx], mid);

    if (seekedVal > val) {
        high = mid;
        return binarySearch(files, fileIdx, high, low, val);
    }
    else {
        low = mid;
        return binarySearch(files, fileIdx, high, low, val);
    }
}

/* This functions takes the result and output it to output.txt */
void outputToFile(unsigned int result) {
    ofstream of;
    of.open("output.txt");
    of << result;
    of.close();
}

/* ============= PRINT FUNCTIONS =============== */
/* Print files for debugging */
void printFile(ifstream* files, int m, int n, int k) {
    cout << "|" << m << "|" << n << "|"<< k << "|" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << "|" << seek(files[i], j) << "|";
        }
        cout << endl;
    }
}

/* Print all variables in serach function */
void printVar(ifstream* files, int fileIdx, int total, vector<int> min, \
        vector<int> max, vector<int> index, int m) {
    cout << "File Index: |" << fileIdx << "|" << endl;
    cout << "Total: |" << total << "|" << endl;
    cout << "Min: ";
    for (int i = 0; i < m; i++) {
        cout << "|" << min[i] << "|";
    }
    cout << endl;
    cout << "Max: ";
    for (int i = 0; i < m; i++) {
        cout << "|" << max[i] << "|";
    }
    cout << endl;
    cout << "Index: ";
    for (int i = 0; i < m; i++) {
        cout << "|" << index[i] << "|";
    }
    cout << endl;
    cout << "File Nums" << endl;
    for (int i = 0; i < m; i++) {
        cout << "|" << seek(files[i], index[i]) - 1<< "|" << seek(files[i], index[i]) << "|" << seek(files[i], index[i] + 1) << "|" << endl;
    }
    cout << endl;
}

/* ================= HELPER FUNCTIONS ================== */
/* Get the info from input.txt */
vector<int> getFileInfo() {
    vector<int> input;
    ifstream ifs;
    string line;

    // Open file
    ifs.open("input.txt");
    if (!ifs) {
        cout << "Error: unable to open file!" << endl;
        exit(1);
    }
    getline(ifs, line);

    // Split string
    stringstream ss(line);
    string temp;
    int tokenValue;
    string token;
    while(getline(ss, token, ',')) {
        input.push_back(atoi(token.c_str()));
    }
    ifs.close();
    return input;
}

/* IMPORTANT: Only works for 1-10 */
string intToString(int integer) {
    string integerStr = "";
    if (integer == 10) {
        integerStr = "10";
        return integerStr;
    }
    else {
        char integerChar = (char) (integer + 48);
        integerStr += integerChar;
        return integerStr;
    }
}

/* Convert the hex string to integer */
unsigned int hexToInt(string hex) {
    unsigned int integer = 0;
    for(int i = 3; i >= 0; i--) {
        int hexInt = (int) hex[i];
        if (hexInt < 0) hexInt += 256;
        unsigned int converted = (hexInt * pow(256, (3 - i)));
        integer += converted;
    }
    return integer;
}

/* Seek the 4Byte data from specified file stream */
unsigned int seek(ifstream& dataFile, int val) {
    char* tempCString = new char[4];
    string tempStr = "";
    dataFile.seekg(4 * val);
    dataFile.read(tempCString, 4);
    for (int i = 0; i < 4; i++) {
        tempStr += tempCString[i];
    }
    unsigned int valSeeked = hexToInt(tempStr);
    delete [] tempCString;
    return valSeeked;
}
