#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "helper.hpp"

using namespace std;

int MST1st(vector< vector<int> >, vector< vector<bool> >&, int);
int MST2nd(vector< vector<int> >, vector< vector<bool> >&, vector<int>&, int, int);
int MST3rd(vector< vector<int> >, vector< vector<bool> >&, vector<int>&, int, int);
vector<int> setupLabels(int);
void addSafeEdges(vector< vector<int> >, vector< vector<bool> >&, vector<int>, int&, int, int&);
void countAndLabel(vector< vector<bool> >, vector<int>&, int n);
void splitLabel(vector< vector<bool> >, vector<int>&, int, int, int);
int addSafeEdgesConnect(vector< vector<int> >, vector< vector<bool> >&, vector<int>, vector<int>&, int, int, int);
void outputFile(int, int, int);

vector< vector<int> > getGraphFromFile(int&);
vector< vector<int> > getGraph(ifstream&, int);
int min(int, int);
void printGraph(vector< vector<int> >, int);
void printMinWeights(vector<int>, int);
void printEdgeConnections(vector< vector<bool> >, int);


int main() {
    int weight1, weight2, weight3;

    // Get the graph from file input.txt
    int n;
    vector< vector<int> > graph = getGraphFromFile(n);

    // Setup a board to check if edges used
    vector< vector<bool> > edgeCheck(n, vector<bool>(n, false));

    // Setup an array to memorize the edges
    vector<int> mem(4, 0);

    // Get the weight of the first minimum spanning tree
    weight1 = MST1st(graph, edgeCheck, n);

    // Get the weight of the second minimum spanning tree
    weight2 = MST2nd(graph, edgeCheck, mem, n, weight1);

    // Get the weight of the third minimum spanning tree
    weight3 = MST3rd(graph, edgeCheck, mem, n, weight2);

    // Edge case for n is 1
    if (n == 1) {
        weight1 = 0;
        weight2 = 0;
        weight3 = 0;
    }
    // Edge case for n is 2
    if (n == 2) {
        weight1 = graph[0][1];
        weight2 = graph[0][1];
        weight3 = graph[0][1];
    }

    /* Output weights to output.txt */
    outputFile(weight1, weight2, weight3);
    return 0;
}

/* Minimum Spanning Tree using Boruvka Algorithm */
int MST1st(vector< vector<int> > graph, vector< vector<bool> > &edgeCheck, int n) {
    int weight = 0;
    // Set up forests counter
    int count = n;
    // Setup labels
    vector<int> labels = setupLabels(n);

    // Connect forests using safe edges
    while (count > 1) {
        // Add Safe Edges, and change edgeCheck, labels, and count
        addSafeEdges(graph, edgeCheck, labels, count, n, weight);
        countAndLabel(edgeCheck, labels, n);
    }
    return weight;
}

/* Set up the labels array to label forests */
vector<int> setupLabels(int n) {
    vector<int> labels(n, 0);
    for (int i = 0; i < n; i++) {
        labels[i] = i;
    }
    return labels;
}

/* Add safe edges to forests */
void addSafeEdges(vector< vector<int> > graph, vector< vector<bool> > &edgeCheck, \
        vector<int> labels, int &count, int n, int &weight) {

    int tempWeight, numConnect = 0, idxU, idxV;
    // This array holds data for minWeight: 2 vertices and weight for each forest.
    vector<int> minWeightData(3 * count, -1);

    // Get minimum weight for each forest
    for (int u = 0; u < n - 1; u++) {
        for (int v = u + 1; v < n; v++) {

            if (labels[u] != labels[v]) {
                tempWeight = graph[u][v];
                idxU = 3 * labels[u];
                idxV = 3 * labels[v];
                if (minWeightData[idxU + 2] == -1 || minWeightData[idxU + 2] > tempWeight) {
                    minWeightData[idxU + 0] = u;
                    minWeightData[idxU + 1] = v;
                    minWeightData[idxU + 2] = tempWeight;
                }
                if (minWeightData[idxV + 2] == -1 || minWeightData[idxV + 2] > tempWeight) {
                    minWeightData[idxV + 0] = u;
                    minWeightData[idxV + 1] = v;
                    minWeightData[idxV + 2] = tempWeight;
                }
            }
        }
    }

    int  u, v;
    // Go through minWeightData
    for (int i = 0; i < count; i++) {
        u = minWeightData[3 * i + 0];
        v = minWeightData[3 * i + 1];
        // If the data exist for the forest, check the edge, and add weight, and reduce count
        if (u != -1 && v != -1 && edgeCheck[u][v] == false) {
            edgeCheck[u][v] = true;
            edgeCheck[v][u] = true;
            weight += minWeightData[3 * i + 2];
            numConnect++;
        }
    }
    count -= numConnect;
    return;
}

void countAndLabel(vector< vector<bool> > edgeCheck, vector<int>& labels, int n) {
    // Reset Labels
    for (int i = 0; i < n; i++) {
        labels[i] = -1;
    }
    int labelCounter = 0;
    int vertexBegin = 0;
    int front;
    bool labeled = false;
    while (!labeled) {
        // Create a queue for depth-first search
        queue<int> toLabel;
        toLabel.push(vertexBegin);
        while (!toLabel.empty()) {
            front = toLabel.front();
            toLabel.pop();
            labels[front] = labelCounter;
            // Check for connected edge, if it is not labeled, push it to queue
            for (int i = 0; i < n; i++) {
                if (edgeCheck[front][i]) {
                    if (labels[i] == -1) {
                        toLabel.push(i);
                    }
                }
            }
        }

        // A for loop to check if all vertex are labeled
        labeled = true;
        for (int i = 0; i < n; i++) {
            if (labels[i] == -1) {
                vertexBegin = i;
                labelCounter++;
                labeled = false;
                break;
            }
        }
    }
}

/* 2nd Minimum Spanning tree by removing an edge in MST and reconstruct */
int MST2nd(vector< vector<int> > graph, vector< vector<bool> > &edgeCheck, \
        vector<int> &mem, int n, int weight1) {
    int weightInc, minWeightInc = -1;
    vector<int> labels(n, 0);
    vector<int> newEdge(2, 0);
    for (int u = 0; u < n - 1; u++) {
        for (int v = u + 1; v < n; v++) {
            if (edgeCheck[u][v]) {
                edgeCheck[u][v] = false;
                edgeCheck[v][u] = false;
                splitLabel(edgeCheck, labels, n, u, v);
                edgeCheck[u][v] = true;
                edgeCheck[v][u] = true;
                weightInc = addSafeEdgesConnect(graph, edgeCheck, labels, newEdge, n, u, v);
                if (minWeightInc == -1) {
                    minWeightInc = weightInc;
                    mem[0] = newEdge[0];
                    mem[1] = newEdge[1];
                    mem[2] = u;
                    mem[3] = v;
                }
                else if (weightInc < minWeightInc) {
                    minWeightInc = weightInc;
                    mem[0] = newEdge[0];
                    mem[1] = newEdge[1];
                    mem[2] = u;
                    mem[3] = v;
                }
            }
        }
    }
    /* Reassign EdgeCheck for the new spanning tree */
    edgeCheck[mem[2]][mem[3]] = false;
    edgeCheck[mem[3]][mem[2]] = false;
    edgeCheck[mem[0]][mem[1]] = true;
    edgeCheck[mem[1]][mem[0]] = true;

    return weight1 + minWeightInc;
}

/* 3rd Minimum Spanning tree by removing an edge in MST and reconstruct */
int MST3rd(vector< vector<int> > graph, vector< vector<bool> > &edgeCheck, \
        vector<int> &mem, int n, int weight2) {

    int weightInc, minWeightInc = -1;
    vector<int> labels(n, 0);
    vector<int> newEdge(2, 0);
    vector<int> weights;
    for (int u = 0; u < n - 1; u++) {
        for (int v = u + 1; v < n; v++) {
            if (edgeCheck[u][v]) {
                edgeCheck[u][v] = false;
                edgeCheck[v][u] = false;
                splitLabel(edgeCheck, labels, n, u, v);
                edgeCheck[u][v] = true;
                edgeCheck[v][u] = true;
                if (u == mem[0] && v == mem[1]) {
                    edgeCheck[mem[2]][mem[3]] = true;
                    edgeCheck[mem[3]][mem[2]] = true;
                }
                weightInc = addSafeEdgesConnect(graph, edgeCheck, labels, newEdge, n, u, v);
                weights.push_back(weightInc);
                if (u == mem[0] && v == mem[1]) {
                    edgeCheck[mem[2]][mem[3]] = false;
                    edgeCheck[mem[3]][mem[2]] = false;
                }
                if (minWeightInc == -1) {
                    minWeightInc = weightInc;
                }
                else if (weightInc < minWeightInc && weightInc >= 0) {
                    minWeightInc = weightInc;
                }
            }
        }
    }
    return weight2 + minWeightInc;
}

/* Label splitted tree */
void splitLabel(vector< vector<bool> > edgeCheck, vector<int>& labels, int n, int u, int v) {
    for (int i = 0; i < n; i++) {
        labels[i] = -1;
    }
    bool labeled = false;
    int top, labelCounter = 0;
    // Create a queue for depth-first search
    stack<int> toLabel;
    toLabel.push(u);
    toLabel.push(v);
    while (!toLabel.empty()) {
        top = toLabel.top();
        if (top == u) labelCounter = 1;
        toLabel.pop();
        labels[top] = labelCounter;
        // Check for connected edge, if it is not labeled, push it to queue
        for (int i = 0; i < n; i++) {
            if (edgeCheck[top][i]) {
                if (labels[i] == -1) {
                    toLabel.push(i);
                }
            }
        }
    }
}

/* Add Edge to connect 2 forest and create another Spanning tree */
int addSafeEdgesConnect(vector< vector<int> > graph, vector< vector<bool> > &edgeCheck, \
        vector<int> labels, vector<int> &newEdge, int n, int oldU, int oldV) {
    int tempWeight = -1;
    int edgeWeight = graph[oldU][oldV];
    for (int u = 0; u < n - 1; u++) {
        for (int v = u + 1; v < n; v++) {
            if (labels[u] != labels[v] && edgeCheck[u][v] == false) {
                if (tempWeight == -1) {
                    newEdge[0] = u;
                    newEdge[1] = v;
                    tempWeight = graph[u][v];
                }
                else if (tempWeight > graph[u][v]) {
                    newEdge[0] = u;
                    newEdge[1] = v;
                    tempWeight = graph[u][v];
                }
            }
        }
    }
    return tempWeight - graph[oldU][oldV];
}

/* Output result to file */
void outputFile(int weight1, int weight2, int weight3) {
    ofstream of;
    of.open("output.txt");
    of << weight1 << endl << weight2 << endl << weight3;
    of.close();
}

/***************************************************************
                            Helper
***************************************************************/
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
