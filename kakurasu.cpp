#include "kakurasu.h"

Kakurasu::Kakurasu(char *fileName)
{
    //solutionFound = false;
    string line, token;
    stringstream ss;
    //unIdentified = 0;
    ifstream in(fileName);
    if (in.is_open())
    {
        getline(in, line);
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        length = atoi(line.c_str());

        solMatrix = new bool*[length + 1];
        for(int i = 1; i <= length; i++)
            solMatrix[i] = new bool[length + 1];

        for(int i = 1; i <= length; i++)
            for(int j = 1; j <= length; j++)
                solMatrix[i][j] = false;

        rowSum = new int[length + 1];
        columnSum = new int[length + 1];
        int count = 0;
        while(getline(in, line))
        {
            if(count > length)
            {
                cout<<"\nArguments more than the size of the matrix";
                break;
            }
            else
            {
                ss.str("");
                ss.clear();
                ss.str(line);
                string str[2];
                int i=0;
                while(getline(ss, token, ','))
                    str[i++] = token;

                if(atoi(str[0].c_str()) == -1 && atoi(str[1].c_str()) != -1)
                {
                    rowSum[++count] = -1;
                    columnSum[count] = atoi(str[1].c_str());
                }
                else if(atoi(str[1].c_str()) == -1 && atoi(str[0].c_str()) != -1)
                {
                    rowSum[++count] = atoi(str[0].c_str());
                    columnSum[count] = -1;
                }
                else if(atoi(str[1].c_str()) == -1 && atoi(str[0].c_str()) == -1)
                {
                    rowSum[++count] = -1;
                    columnSum[count] = -1;
                }
                else
                {
                    rowSum[++count] = atoi(str[0].c_str());
                    columnSum[count] = atoi(str[1].c_str());
                }
            }
        }
    }
    in.close();

    rowVector = new vector<vector<bitset<9>>*>;
    columnVector = new vector<vector<bitset<9>>*>;
    for(int i = 0; i<=length - 1; i++)
    {
        rowVector->push_back((new vector<bitset<9>>));
        columnVector->push_back((new vector<bitset<9>>));
    }
    T = 0;
    bValue = 0.1;
}


void Kakurasu::printSolMatrix()
{
    for(int i = 1; i <= length; i++)
    {
        cout<<"\n";
        for(int j = 1; j <= length; j++)
            cout<<"\t"<<solMatrix[i][j];
        cout<<"\t"<<rowSum[i];
    }
    cout<<"\n";
    for(int i = 1; i <= length; i++)
        cout<<"\t"<<columnSum[i];

}


void Kakurasu::getAllCombinations()
{
    int sum = 0;
    for(long int i = 1; i<=(pow(2,length) - 1); i++)
    {
        sum = 0;
        bitset<9> set(i);

        for(int j = 1; j<=length; j++)
        {
            sum = sum + set[j - 1] * j;
        }
        for(int j = 1; j<=length; j++)
        {
            if(sum == rowSum[j])
            {
                rowVector->at(j - 1)->push_back(set);
                //rowCount[j]+=1;
            }
            if(sum == columnSum[j])
            {
                columnVector->at(j - 1)->push_back(set);
                //columnCount[j]+=1;
            }
            if(rowSum[j] == -1)
            {
                rowVector->at(j - 1)->push_back(set);
                //rowCount[j]+=1;
            }
            if(columnSum[j] == -1)
            {
                columnVector->at(j - 1)->push_back(set);
                //columnCount[j]+=1;
            }
        }
    }
    cout<<"\n\n-------------- Initial Combinations:----------------- \n";
    cout<<"\nFor all rows\n";
    int count = 0;
    for(vector<vector<bitset<9>>*>::iterator it = rowVector->begin() ; it != rowVector->end(); ++it)
    {
        cout<<"\nFor row : "<<++count<<"\n";
        for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            cout<<*itIn<<"\t";
    }

    cout<<"\nFor all columns\n";
    count = 0;
    for(vector<vector<bitset<9>>*>::iterator it = columnVector->begin() ; it != columnVector->end(); ++it)
    {
        cout<<"\nFor column : "<<++count<<"\n";
        for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            cout<<*itIn<<"\t";
    }
}


void Kakurasu::simulatedAnnealing()
{
    cout<<"\nnStarting with Simulated Annealing";
    cout<<"\nSeting the initial state\n";

    //Going to choose row constraints
    //hOfY = 1;
    double smallT = 0.0;
    bool didWeGetALowerHOfY = false;
    struct Children parentStruct;
    vector<bitset<9>> tempcolumnVector;
    vector<int> rowIndexGot;
    int hashKey = MAX_INT_VALUE;
    //Initially assigning the first element to the solver
    for(vector<vector<bitset<9>>*>::iterator it = rowVector->begin() ; it != rowVector->end(); ++it)
    {
        parentStruct.singleRowWiseAssign.push_back((*it)->at(0));
        parentStruct.hashKey = parentStruct.hashKey * 10 + 0;
        parentStruct.indexNumVector.push_back(0);
    }
    while(hashTable.find(parentStruct.hashKey) == hashTable.end())
    {
        didWeGetALowerHOfY = false;
        smallT++;
        T = A_VALUE - bValue * smallT;
        cout<<"\nGot the hashKey , T : "<<parentStruct.hashKey<<" , "<<T;

        hashTable.insert(parentStruct.hashKey);
        tempcolumnVector.clear();
        //Get equivalent columns for this combination
        for(int i = 0; i<=length - 1; i++)
        {
            bitset<9> tempBitset;
            tempBitset.reset();
            int indexColBitset = 0;
            for(vector<bitset<9>>::iterator rowIt = parentStruct.singleRowWiseAssign.begin() ;
                rowIt != parentStruct.singleRowWiseAssign.end(); ++rowIt)
            {
                tempBitset[indexColBitset++] = (*rowIt)[i];
            }
            cout<<"\nGot Column from the parent row : "<<tempBitset;
            tempcolumnVector.push_back(tempBitset);
        }

        cout<<"\nChecking if this is the solution";
        int countMatch = 0;
        int tempIndex = 0;
        for(vector<vector<bitset<9>>*>::iterator it = columnVector->begin() ; it != columnVector->end(); ++it)
        {
            for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            {
                //cout<<"\ntempcolumnVector[tempIndex].to_string() == (*itIn).to_string() : "<<
                      //tempcolumnVector[tempIndex].to_string()<<","<<(*itIn).to_string();
                if(tempcolumnVector[tempIndex].to_string() == (*itIn).to_string())
                {
                    countMatch++;
                }
            }
            tempIndex++;
        }
        if(countMatch == length)
        {
            cout<<"\nMatch Found";
            exit(0);
        }


        //find hOfX
        //hOfX = 0;
        tempIndex = 0;
        int countAll = 0, countMin = MAX_INT_VALUE;
        for(vector<vector<bitset<9>>*>::iterator it = columnVector->begin() ; it != columnVector->end(); ++it)
        {
            for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
            {
                for(int i = 0; i <= length - 1; i++)
                {
                    if(tempcolumnVector[tempIndex][i] != (*itIn)[i])
                    {
                        countAll++;
                    }
                }
                if(countAll<countMin)
                {
                    countMin = countAll;
                }
                countAll = 0;
            }
            parentStruct.hOfX = parentStruct.hOfX + countMin;
            cout<<"\ncountMin = "<<countMin;
            countMin = MAX_INT_VALUE;
            tempIndex++;
            cout<<"\nhOfx of Parent : "<<parentStruct.hOfX;
        }

        //find childrens
        vector<struct Children> childsVector;
        do
        {
            childsVector.clear();
            int checkIterations = 0;
            for(vector<vector<bitset<9>>*>::iterator it = rowVector->begin() ; it != rowVector->end(); ++it)
            {
                default_random_engine genRowIndex(random_device{}());
                uniform_int_distribution<int> distRowIndex(0, (int)((*it)->size() - 1));
                int rowIndex = distRowIndex(genRowIndex);

                cout<<"\nAt rowIndex got : "<<rowIndex<<" , element selected : "<<(*it)->at(rowIndex);
                struct Children cTemp;
                cout<<"\nChild row wise : ";
                for(int j = 0; j < length; j++)
                {
                    if(checkIterations == j)
                    {
                        cout<<"\n"<<(*it)->at(rowIndex);
                        cTemp.singleRowWiseAssign.push_back((*it)->at(rowIndex));
                        cTemp.indexNumVector.push_back(rowIndex);
                    }
                    else
                    {
                        cout<<"\n"<<parentStruct.singleRowWiseAssign[j];
                        cTemp.singleRowWiseAssign.push_back(parentStruct.singleRowWiseAssign[j]);
                        cTemp.indexNumVector.push_back(parentStruct.indexNumVector[j]);
                    }
                }
                int cTempIndex = 0;
                //calculating hash of this combination
                for(vector<bitset<9>>::iterator it = cTemp.singleRowWiseAssign.begin();
                    it != cTemp.singleRowWiseAssign.end(); ++it)
                {
                    cTemp.hashKey = cTemp.hashKey * 10 + cTemp.indexNumVector[cTempIndex];
                    cTempIndex++;
                }
                cout<<"\nHash Key for the Child above : "<<cTemp.hashKey<<"\n";
                childsVector.push_back(cTemp);
                checkIterations++;
            }

            double tempOfHYGot = MAX_INT_VALUE;
            //Calculate the hOfY for all these childrens
            for(vector<struct Children>::iterator itChild = childsVector.begin();
                itChild != childsVector.end(); ++itChild)
            {
                tempcolumnVector.clear();
                //Get equivalent columns for this combination
                for(int i = 0; i<=length - 1; i++)
                {
                    bitset<9> tempBitset;
                    tempBitset.reset();
                    int indexColBitset = 0;
                    for(vector<bitset<9>>::iterator rowIt = (*itChild).singleRowWiseAssign.begin() ;
                        rowIt != (*itChild).singleRowWiseAssign.end(); ++rowIt)
                    {
                        tempBitset[indexColBitset++] = (*rowIt)[i];
                    }
                    cout<<"\nChild Column -> "<<tempBitset;
                    tempcolumnVector.push_back(tempBitset);
                }

                double tempOfHY = 0;
                tempIndex = 0;
                countAll = 0;
                countMin = MAX_INT_VALUE;
                for(vector<vector<bitset<9>>*>::iterator it = columnVector->begin() ; it != columnVector->end(); ++it)
                {
                    for(vector<bitset<9>>::iterator itIn = (*it)->begin() ; itIn != (*it)->end(); ++itIn)
                    {
                        for(int i = 0; i <= length - 1; i++)
                        {
                            if(tempcolumnVector[tempIndex][i] != (*itIn)[i])
                            {
                                countAll++;
                            }
                        }
                        if(countAll<countMin)
                        {
                            countMin = countAll;
                        }
                        countAll = 0;
                    }
                    tempOfHY = tempOfHY + countMin;
                    cout<<"\ncountMin = "<<countMin;
                    countMin = MAX_INT_VALUE;
                    tempIndex++;
                    cout<<"\ntempHOfY : "<<tempOfHY;
                }

                if(tempOfHY < parentStruct.hOfX && (hashTable.find((*itChild).hashKey) == hashTable.end()))
                {
                    parentStruct = (*itChild);
                    cout<<"\n\nParent selected : "<<parentStruct.hashKey<<"\n";
                    parentStruct.hOfY = tempOfHY;
                    didWeGetALowerHOfY = true;
                }
                if(tempOfHYGot > tempOfHY)
                    tempOfHYGot = tempOfHY;
            }
            if(!didWeGetALowerHOfY)
            {
                cout<<"\n\nParent not selected\n";
                //Go through the child again and set any whose hash is not there
                default_random_engine generatorUniformReal(random_device{}());
                uniform_real_distribution<double> uniformRealDistribution(0.0, 1.0);
                double uniformSample = uniformRealDistribution(generatorUniformReal);
                cout<<"\nuniformSample , exp : "<<uniformSample<<", "<<exp((parentStruct.hOfX - tempOfHYGot)/smallT);
                if(uniformSample < exp((parentStruct.hOfX - tempOfHYGot)/smallT))
                {
                    for(vector<struct Children>::iterator itChild = childsVector.begin();
                        itChild != childsVector.end(); ++itChild)
                    {
                        if(hashTable.find((*itChild).hashKey) == hashTable.end())
                        {
                            parentStruct = (*itChild);
                            cout<<"\n\nParent selected after unform : "<<parentStruct.hashKey<<"\n";
                            didWeGetALowerHOfY = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                cout<<"\n\nUniform Sampling is also less\n";
                break;
            }
            if(didWeGetALowerHOfY)
                break;
        }while(true);
    }
}
