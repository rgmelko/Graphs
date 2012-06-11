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
        vector< int > SubgraphList;
        vector< vector< tuple<int, int, int> > > RealSpaceCoordinates;

};

void ReadGraphsFromFile(vector< graph > & graphList, string & file);
void WriteGraphsToFile(vector< graph > & graphList, string & file);
