#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

class Graph 
{

    public :
        //vector< pair< int, int > > AdjacencyList;
        int LatticeConstant;
        int Order;
        int Identifier;
        vector< int > SubgraphList;

        Graph();
        Graph(int, int, int, vector< int > & );

        Graph& operator=(const Graph & other);
        bool operator==(const Graph & other);

//        Graph GetGraphFromFile(const int Identifier, const string & file);
};

class SiteGraph : public Graph
{
    private :
        vector< pair<int, int> > Sites;

    public :       
        SiteGraph();
        SiteGraph(vector< pair<int, int> > & , int, int, int, vector< int > & );
        void AddSite(int xIndex, int yIndex);
        void RemoveSite(int xIndex, int yIndex);
        bool CheckForSite(int xIndex, int yIndex);
        int SiteDegree(int xIndex, int yIndex);
        bool operator==(const SiteGraph & other);
        SiteGraph & operator=(const SiteGraph & other);

};

class BondGraph : public Graph
{
    private :
        vector< pair< pair<int, int>, pair<int, int> > > Bonds;

    public :
        BondGraph();
        BondGraph(vector< pair< pair<int, int>, pair<int,int> > > & , int, int, int, vector< int > & );
        void AddBond(pair<int , int> , pair< int, int> );
        void RemoveBond(pair<int , int> , pair< int, int> );
        bool CheckForBond(pair<int , int> , pair< int, int> );
        int BondCount(pair<int , int> , pair< int, int> );
        bool operator==(const BondGraph & other);
        BondGraph & operator=(const BondGraph & other);
};

//void ReadGraphsFromFile(vector< Graph > & GraphList, const string & file);
//void WriteGraphsToFile(vector< Graph > & GraphList, string file);
