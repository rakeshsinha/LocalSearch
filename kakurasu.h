#ifndef KAKURASU_H
#define KAKURASU_H

#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>
#include <set>
#include <list>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <random>

#define ROW_COL_SWITCH_PROB 0.5
#define MAX_INT_VALUE 2147483647
#define A_VALUE 10000

using namespace std;
struct elementId { int indices[2];};
struct bits { bitset<9> e;};

inline bool operator<(const bits & e1, const bits& e2)
{
    bitset<9> tp1 = e1.e;
    bitset<9> tp2 = e2.e;
    return ((int)tp1.to_ulong() < (int)tp2.to_ulong());
}

inline bool operator<(const elementId& e1, const elementId& e2)
{
    if(e1.indices[0] == e2.indices[0])
        return e1.indices[1]<e2.indices[1];
    return e1.indices[0]<e2.indices[0];
}

struct fValue
{
    int rindex;
    int cindex;
    float scoreValue;
};

struct Children
{
    Children()
    {
        hashKey = 0;
        hOfX = 0;
        hOfY = 0;
    }

    vector<bitset<9>> singleRowWiseAssign;
    vector<int> indexNumVector;
    int hashKey;
    double hOfX;
    double hOfY;
};

class Kakurasu
{
public:
    Kakurasu(char*);
    int length;
    int *rowSum;
    int *columnSum;
    //int hOfX;
    //int hOfY;
    float bValue;
    double T;
    set<int> hashTable;
    vector<vector<bitset<9>>*> *rowVector;
    vector<vector<bitset<9>>*> *columnVector;
    bool **solMatrix;
    void getAllCombinations();
    void printSolMatrix();
    void simulatedAnnealing();




/*
    int unIdentified;
    int *rowCount;
    int *columnCount;
    int *rSortedIndex;
    int *cSortedIndex;
    void sortIndex(int *, int *);
    int **checkMatrix;
    void ACAlgorithm(vector<vector<bitset<9>>*> *, vector<vector<bitset<9>>*> *, int **);
    set<elementId> priorityQueue;
    set<elementId> listOfVariables;
    int **fMatrix;
    int unidentifiedVariables(int **, int **);
    list<fValue> fValueList;
    void stm(list<fValue>& , int **, int **);
    float FValueCalculate(int, int, list<fValue>);
    set<elementId> elementsToCheck;
    list<int> col_position;
    list<int> row_position;
    set<bits> colVariableCombinations;
    set<bits> colVariableCombinations1;
    set<bits> rowVariableCombinations;
    set<bits> rowVariableCombinations1;
    void searchalgorithm(vector<vector<bitset<9>>*> *, vector<vector<bitset<9>>*> *, int **, int ** , list<fValue>);
    void printCheckMatrix(int **);
    void printfMatrix(int **);
    bool solutionFound;*/
};

#endif // KAKURASU_H
