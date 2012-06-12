#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class graph 
{

    public :
        vector< pair< int, int > > AdjacencyList;
        int LatticeConstant;
        int NumberSites;
        int NumberBonds;
        int Identifier;
        vector< int > SubgraphList;
        vector< vector< pair<int, int> > > RealSpaceCoordinates;

};

void ReadGraphsFromFile(vector< graph > & graphList, string & file);
void WriteGraphsToFile(vector< graph > & graphList, string file);
