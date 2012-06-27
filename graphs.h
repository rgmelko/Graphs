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
        vector< pair<int, int> > AdjacencyList;
        vector< pair<int, int> > SubgraphList;

        Graph();
        Graph(int, int, int, vector< pair<int, int> > & );

        Graph& operator=(const Graph & );
        bool operator==(const Graph & );

};

class SiteGraph : public Graph
{
    public :       
        vector< pair<int, int> > Sites;
        SiteGraph();
        SiteGraph(vector< pair<int, int> > & , int, int, int, vector< pair<int, int> > & );
        void AddSite(int , int );
        void RemoveSite(int , int );
        bool CheckForSite(int , int );
        int SiteDegree(int , int );
        void MakeCanonical();
        void PrintGraph();
        void GenerateAdjacencyList();
        bool operator==(const SiteGraph & );
        SiteGraph & operator=(const SiteGraph & );

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
        void PrintGraph();
        void GenerateAdjacencyList();
        bool operator==(const BondGraph & );
        BondGraph & operator=(const BondGraph &);
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

void ConstructSiteBasedGraphs(vector< vector< SiteGraph > > & , int );
void ConstructBondBasedGraphs(vector< vector< BondGraph > > & , int );

void ConstructRectangularSiteGraphs( vector< vector< SiteGraph > > & , unsigned int , unsigned int );
void ConstructRectangularSiteGraphs( vector< vector< SiteGraph > > & , unsigned int );

//void ReadGraphsFromFile(vector< Graph > & GraphList, const string & file);
void WriteGraphsToFile(vector< SiteGraph > & , string );
void WriteGraphsToFile(vector< vector< SiteGraph > > & , string );
void WriteGraphsToFile(vector< BondGraph > & , string );
void WriteGraphsToFile(vector< vector< BondGraph > > & GraphList, string file);
