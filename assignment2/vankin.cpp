#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int vankin(vector< vector<int> >, int);
void outputToFile(int);
int getN(ifstream&);
vector< vector<int> > getArray(ifstream&, int);
int max(int, int);
void printBoard(vector< vector<int> >, int);

int main() {
    // Open input.txt file
    ifstream input;
    input.open("input.txt");
    if (!input) {
        cout << "Error: unable to open file" << endl;
        exit(1);
    }

    // Get all the values from the files
    int n = getN(input);
    vector< vector<int> > board = getArray(input, n);
    input.close();

    // Vankin to get max sum
    int max = vankin(board, n);

    // Output max sum to output.txt
    outputToFile(max);

    return 0;
}

/* vankin function to find the max sum of the vankin board */
int vankin(vector< vector<int> > board, int n) {
    int maxSum;
    // Temp array to store temporary highest sum value
    vector<int> s(n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // start with number on right bottom corner,
            // assign it to the temp array s, and assign it to maxSum
            if (i == 0 && j == 0) {
                s[0] = board[0][0];
                maxSum = s[0];
            }
            // If the number is on bottom row, add the max value of (number to
            // the right, 0)
            else if (i == 0) {
                s[j] = board[0][j] + max(s[j - 1], 0);
            }
            // If the nubmer is on the right column, add the max value of (number
            // beneath, 0)
            else if (j == 0) {
                s[0] = board[i][0] + max(s[0], 0);
            }
            // If the number is not on the right-most column, or bottom-most row,
            // add the max value of (number beneath, number to the right)
            else {
                s[j] = board[i][j] + max(s[j], s[j - 1]);
            }
            maxSum = max(maxSum, s[j]);
        }
    }
    return maxSum;
}

/* Output the integer to output.txt */
void outputToFile(int result) {
    ofstream of;
    of.open("output.txt");
    of << result;
    of.close();
}

/*=============================================================
====================== Helper Functions =======================
=============================================================*/

/* Get the first value of input.txt and convert it to int */
int getN(ifstream& input) {
    string firstLine;
    getline(input, firstLine);
    return atoi(firstLine.c_str());
}

/* Use value n, to get all values from input.txt and put it
   inside the 2-d vector */
vector< vector<int> > getArray(ifstream& input, int n) {
    string line, token;
    stringstream lineStream;
    int temp;
    // Setup 2-d vector
    vector< vector<int> > board(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        getline(input, line);
        lineStream.str(line);

        //Split line into tokens using delimiter ','
        for (int j = 0; j < n; j++) {
            getline(lineStream, token, ',');
            // convert the token to int and store to board
            board[n - i - 1][n - j - 1] = atoi(token.c_str());
        }
        lineStream.clear();
    }
    return board;
}

/* return the max between 2 integers */
int max(int a, int b) {
    return ((a > b) ? a : b);
}

/* debug function to print the board */
void printBoard(vector< vector<int> > board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
