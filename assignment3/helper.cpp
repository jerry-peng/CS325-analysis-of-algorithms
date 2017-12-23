#include "helper.hpp"

/* Get the value of n, and setup 2-d vector from
   file input.txt */
vector< vector<int> > getGraphFromFile(int& n) {
    string line;

    // Open file, if file does not exist, exit program
    ifstream ifs;
    ifs.open("input.txt");
    if (!ifs) {
        cout << "Error: file not found" << endl;
        exit(1);
    }

    // Get first line to initialize 2-d vector
    getline(ifs, line);
    n = atoi(line.c_str());

    // initialize vector
    vector< vector<int> > graph = getGraph(ifs, n);

    ifs.close();
    return graph;
}

/* Use value n, to get all values from input.txt and put it
   inside the 2-d vector */
vector< vector<int> > getGraph(ifstream& ifs, int n) {
    string line, token;
    stringstream lineStream;
    int temp;
    // Setup 2-d vector
    vector< vector<int> > graph(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        getline(ifs, line);
        lineStream.str(line);

        //Split line into tokens using delimiter ','
        for (int j = 0; j < n; j++) {
            getline(lineStream, token, ',');
            // convert the token to int and store to graph board
            graph[i][j] = atoi(token.c_str());
        }
        lineStream.clear();
    }
    return graph;
}

/*********************************************************
                        DEBUG
*********************************************************/
/* debug function to print the graph */
void printGraph(vector< vector<int> > graph, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

/* debug function to print the minWeights array */
void printMinWeights(vector<int> minWeightData, int count) {
    int weightInLine = 5;
    for (int i = 0; i < count; i++) {
        cout << minWeightData[3 * i] << "|"
             << minWeightData[3 * i + 1] << "|"
             << minWeightData[3 * i + 2] << "||";
        if (i % weightInLine == 4) {
            cout << endl;
        }
    }
    cout << endl;
}

/* print edges that has connections */
void printEdgeConnections(vector< vector<bool> > edgeCheck, int n) {
    int counter = 0;
    for (int u = 0; u < n - 1; u++) {
        for (int v = u + 1; v < n; v++) {
            if (edgeCheck[u][v]) {
                cout << "EDGE: " << u << "|" << v << " ";
                counter ++;
                if (counter % 5 == 4) {
                    cout << endl;
                }
            }
        }
    }
    cout << endl;
}
