#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

class Graph 
{

    public :
        //vector< pair< int, int > > AdjacencyList;
        int LatticeConstant;
        int Order;
        int Identifier;
        vector< pair<int, int> > SubgraphList;

        Graph();
        Graph(int, int, int, vector< pair<int, int> > & );

        Graph& operator=(const Graph & other);
        bool operator==(const Graph & other);

//        Graph GetGraphFromFile(const int Identifier, const string & file);
};

class SiteGraph : public Graph
{
    public :       
        vector< pair<int, int> > Sites;
        SiteGraph();
        SiteGraph(vector< pair<int, int> > & , int, int, int, vector< pair<int, int> > & );
        void AddSite(int xIndex, int yIndex);
        void RemoveSite(int xIndex, int yIndex);
        bool CheckForSite(int xIndex, int yIndex);
        int SiteDegree(int xIndex, int yIndex);
        void MakeCanonical();
        void PrintGraph();
        bool operator==(const SiteGraph & other);
        SiteGraph & operator=(const SiteGraph & other);

};

class BondGraph : public Graph
{
    public :
        vector< pair< pair<int, int>, pair<int, int> > > Bonds;
        BondGraph();
        BondGraph(vector< pair< pair<int, int>, pair<int,int> > > & , int, int, int, vector< pair<int, int> > & );
        void AddBond(pair<int , int> , pair< int, int> );
        void RemoveBond(pair<int , int> , pair< int, int> );
        bool CheckForBond(pair<int , int> , pair< int, int> );
        int BondCount(pair<int , int> , pair< int, int> );
        void MakeCanonical();
        bool operator==(const BondGraph & other);
        BondGraph & operator=(const BondGraph & other);
};

class Dihedral
{
    public :
        int element;
        Dihedral();
        Dihedral(int);
        void operator() (pair<int,int> & );
        void operator() (pair< pair<int,int>, pair<int,int> > & );
} Transform;

void ConstructSiteBasedGraphs(vector< vector< SiteGraph > > & graphs, int FinalOrder);

void ConstructRectangularSiteGraphs( vector< vector< SiteGraph > > & graphs, unsigned int Finalm, unsigned int Finaln);

//void ReadGraphsFromFile(vector< Graph > & GraphList, const string & file);
void WriteGraphsToFile(vector< SiteGraph > & GraphList, string file);
void WriteGraphsToFile(vector< vector< SiteGraph > > & GraphList, string file);
