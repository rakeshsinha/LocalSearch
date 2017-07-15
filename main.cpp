#include <iostream>
#include "kakurasu.h"

using namespace std;

int main(int argc, char *argv[])
{
    Kakurasu *kakurasu;
    if(argc == 2)
    {
        kakurasu = new Kakurasu(argv[1]);
    }
    else
    {
        cout<<"Exceeds or needs Parameter\n";
        cout<<"Usage : ./kakurasu <filename>\n"
                "Make sure there is no space in the filename\n";
        return 0;
    }

    cout<<"\n\nPrinting the Matrix\n";
    kakurasu->printSolMatrix();
    kakurasu->getAllCombinations();
    kakurasu->simulatedAnnealing();
    cout<<"\n\n";
    return 0;
}
