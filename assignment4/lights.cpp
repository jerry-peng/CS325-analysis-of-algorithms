#include "./cnf2sat.hpp"

using namespace std;

vector<pair<int, int> > createClauses(int, int, vector<bool>, vector<int>, vector<int>);
void outputFile(bool);
void getDataFromFile(int&, int&, vector<bool>&, vector<int>&, vector<int>&);
void getNandM(ifstream&, int&, int&);
void getLightsArray(ifstream&, int, vector<bool>&);
void getSwitchesArr(ifstream&, int, int, vector<int>&, vector<int>&);
void printFileData(int, int, vector<bool>, vector<int>, vector<int>);
void printClauses(int, vector<pair<int, int> >);

int main() {
    int n, m;
    vector<bool> lightsArr;
    vector<int> switchesArr1, switchesArr2;

    // Get the clauses pairs for the switches
    getDataFromFile(n, m, lightsArr, switchesArr1, switchesArr2);

    // Print the data from file
    //printFileData(n, m, lightsArr, switchesArr1, switchesArr2);

    vector<pair<int, int> > clauses = \
        createClauses(n, m, lightsArr, switchesArr1, switchesArr2);

    // Print the value in the clauses
    //printClauses(m, clauses);

    // Pass clauses to the 2-SAT blackbox
    bool result = satisfiable(clauses);

    // Output result to output.txt
    outputFile(result);

    return 0;
}

vector<pair<int, int> > createClauses(int n, int m, vector<bool> lightsArr,\
        vector<int> switchesArr1, vector<int> switchesArr2) {

    vector<pair<int, int> > clauses(2 * m, pair<int, int>(0, 0));
    // fill data into the clauses
    for (int i = 0; i < m; i++) {
        if (lightsArr[i]) {
            clauses[2 * i].first = switchesArr1[i];
            clauses[2 * i].second = switchesArr2[i];
            clauses[2 * i + 1].first = -switchesArr1[i];
            clauses[2 * i + 1].second = -switchesArr2[i];
        }
        else {
            clauses[2 * i].first = switchesArr1[i];
            clauses[2 * i].second = -switchesArr2[i];
            clauses[2 * i + 1].first = -switchesArr1[i];
            clauses[2 * i + 1].second = switchesArr2[i];
        }
    }
    return clauses;
}

void outputFile(bool result) {
    ofstream ofs;
    ofs.open("output.txt");
    if (result) {
        ofs << "yes";
    }
    else {
        ofs << "no";
    }
    ofs.close();
}

/*=============================================================
====================== Helper Functions =======================
=============================================================*/

/* Get data from file and store it into variables passed by reference */
void getDataFromFile(int &n, int &m, vector<bool> &lightsArr,\
        vector<int> &switchesArr1, vector<int> &switchesArr2) {
    ifstream ifs;

    // Open input.txt file
    ifs.open("input.txt");
    if (!ifs) {
        cout << "Error: unable to open file" << endl;
        exit(1);
    }

    // Get n (number of switches) and m (number of lights)
    getNandM(ifs, n, m);

    // Set up arrays/vectors for more data from the input.txt
    vector<bool> arr1(m, -1);
    vector<int> arr2(m, -1);
    vector<int> arr3(m, -1);
    lightsArr = arr1;
    switchesArr1 = arr2;
    switchesArr2 = arr3;

    // Get array of initial light status (on or off)
    getLightsArray(ifs, m, lightsArr);

    // Get the switch connections for each lights
    getSwitchesArr(ifs, n, m, switchesArr1, switchesArr2);

    // close filestream
    ifs.close();
}

/* Get n and m from first line of input.txt and store it into variables
    passed by reference */
void getNandM(ifstream &ifs, int &n, int &m) {
    string line, token;
    stringstream ss;

    getline(ifs, line);
    ss.str(line);

    getline(ss, token, ',');
    n = atoi(token.c_str());

    getline(ss, token, ',');
    m = atoi(token.c_str());
}

/* Get the status of lights (on or off) from the second line of
    input.txt, and store it into variables passed by reference */
void getLightsArray(ifstream &ifs, int m, vector<bool> &lightsArr) {
    string line, token;
    stringstream ss;
    int switchVal;

    getline(ifs, line);
    ss.str(line);

    for (int i = 0; i < m; i++) {
        getline(ss, token, ',');
        switchVal = atoi(token.c_str());
        lightsArr[i] = switchVal ? true : false;
    }
}

void getSwitchesArr(ifstream& ifs, int n, int m, vector<int> &switchesArr1,\
        vector<int> &switchesArr2) {
    string line, token;
    stringstream ss;
    int connLight;

    for (int i = 0; i < n; i++) {
        getline(ifs, line);
        ss.str(line);

        while(getline(ss, token, ',')) {
            connLight = atoi(token.c_str()) - 1;
            if (switchesArr1[connLight] == -1) {
                switchesArr1[connLight] = i + 1;
            }
            else {
                switchesArr2[connLight] = i + 1;
            }
        }

        ss.clear();
    }
}

/*=============================================================
======================= Debug Functions =======================
=============================================================*/
void printFileData(int n, int m, vector<bool> lightsArr,\
        vector<int> switchesArr1, vector<int> switchesArr2) {
    cout << "n |" << n << "|" << endl;
    cout << "m |" << m << "|" << endl;
    cout << "Light status |";
    for (int i = 0; i < m; i++) {
        cout << lightsArr[i] << "|";
    }
    cout << endl;
    cout << "LightConnection1 |";
    for (int i = 0; i < m; i++) {
        cout << switchesArr1[i] << "|";
    }
    cout << endl;
    cout << "LightConnection2 |";
    for (int i = 0; i < m; i++) {
        cout << switchesArr2[i] << "|";
    }
    cout << endl;
}

void printClauses(int m, vector<pair<int, int> > clauses) {
    cout << "Clauses |";
    for (int i = 0; i < 2 * m; i++) {
        cout << clauses[i].first << "|";
    }
    cout << endl << "Clauses |";
    for (int i = 0; i < 2 * m; i++) {
        cout << clauses[i].second << "|";
    }
    cout << endl;
}
