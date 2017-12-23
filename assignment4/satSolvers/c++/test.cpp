#include "./cnf2sat.hpp"

int main() {
    int numClauses = 2;
    int a[] = {-2, 2};
    int b[] = {-2, 2};
    vector<pair<int, int> > test; 
    for(int i = 0; i < numClauses; i++) {
        test.push_back(make_pair(a[i],b[i]));
    }
    cout << "Expected: 0\nActual: " << satisfiable(test) << endl;
    test.clear();

    numClauses = 7;
    int c[] = {1, -2, -1, 3, -3, -4, -3};
    int d[] = {2, 3, -2, 4, 5, -5, 4};

    for(int i = 0; i < numClauses; i++) {
        test.push_back(make_pair(c[i],d[i]));
    }
    cout << "\nExpected: 1\nActual: " << satisfiable(test) << endl;
    return 0;
}
