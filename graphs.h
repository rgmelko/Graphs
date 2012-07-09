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

/*! \class Graph

\brief A superclass containing information about any type of graph

Defines parameters describing a graph without reference to the "type" (site- or bond-based) of the graph, including number of distinct embeddings, number of elements, a unique identifier, the connectivity of elements, and a list of subgraphs. 

\sa SiteGraph, BondGraph
*/

class Graph 
{

    public :
        int LatticeConstant;
        int Order;
        int Identifier;
        vector< pair<int, int> > AdjacencyList;
        vector< pair<int, int> > SubgraphList;

        /*! Initializes a default graph object
        */
        Graph();
        /*! Initializes a general graph object with the given parameters
        \param LatticeConstant The number of distinct ways to embed this graph in the lattice
        \param Order The number of elements in the graph
        \param Identifier The unique number which identifies this specific graph
        \param SubGraphList A list of the subgraphs of the graph, and the number of times each subgraph is embeddable in the graph
        */
        Graph(int, int, int, vector< pair<int, int> > & );

        /*! Assigns the value of one Graph to another
        \param other A reference to the graph which is being copied
        \returns A reference to a new graph, which is a copy of the referenced graph
        */
        Graph& operator=(const Graph & );
        /* Determines whether two Graphs have the same descriptors
        \param other A reference to the graph which is being compared
        \returns A boolean - true if the descriptors are the same, false otherwise 
        */
        bool operator==(const Graph & );

};

/*! \class SiteGraph
\brief A sublcass of Graph which describes site-based graphs

A site-based graph is one where the connections between sites are less important than the presence of the site at all. The graph is fully described by which sites are or are not present. If two sites are adjacent and present, they must be connected. This class has one additional member: Sites, which describes the real-space locations of the sites which are present
*/
class SiteGraph : public Graph
{
    public :       
        vector< pair<int, int> > Sites;
        /*! \brief Initializes a default SiteGraph
        */
        SiteGraph();
        /*! \brief Initializes a SiteGraph with the given parameters
        \param Sites The list of the coordinates of the sites present in the graph
        \param Identifier The unique number identifying the graph
        \param Order The number of sites in the graph
        \param LatticeConstant The number of distinct ways to embed this graph in the lattice
        \param SubgraphList The list of subgraphs of this graph, and the number of ways each can be embedded in the graph
        */
        SiteGraph(vector< pair<int, int> > & , int, int, int, vector< pair<int, int> > & );
        /*! \brief Adds a site to the list of sites
        \param xIndex The x coordinate of the site
        \param yIndex The y coordinate of the site
        */
        void AddSite(int , int );
        /*! \brief Removes a site to the list of sites
        \param xIndex The x coordinate of the site
        \param yIndex The y coordinate of the site
        */
        void RemoveSite(int , int );
        /*! \brief Looks for a site in the list of sites
        \param xIndex The x coordinate of the site
        \param yIndex The y coordinate of the site
        */
        bool CheckForSite(int , int );
        /*! \brief Finds the number of sites adjacent to this site in the list of sites
        \param xIndex The x coordinate of the site
        \param yIndex The y coordinate of the site
        */
        int SiteDegree(int , int );
        /*! \brief Transforms the graph into its canonical representation
        Uses the Dihedral class to check all representations of the graph, attemping to maximise (xCoordinates)*Order + yCoordinates. Whichever representation maximises this number is unique and canonical.
        */
        void MakeCanonical();
        /*! \brief Dumps the descriptors of the graph to standard out
        */
        void PrintGraph();
        /*! \brief Creates a one-integer label for each site and determines which sites are connected
        Assigns each site a label, moving east first, then north. Then determines whether the sites directly east and north of the current site are in the graph, and if so, notes that each site is connected. 
        */
        void GenerateAdjacencyList();
        /*! \brief Determines the number of distinct ways to embed the graph in the lattice
        Applies each element of the relevant dihedral group to the graph, and determines how many of the results are distinct from each other (there is no way to add an integer to all coordinates and get the other graph).
        */
        void FindLatticeConstant();
        bool operator==(const SiteGraph & );
        SiteGraph & operator=(const SiteGraph & );
};

/*! \class BondGraph
\brief A sublcass of Graph which describes bond-based graphs

A bond-based graph is one where the connections between sites are more important than the presence of the sites themselves. The graph is fully described by which sites are or are not connected. If two sites are adjacent and present, they may or may not be connected. This class has one additional member: Bonds, which describes the real-space locations of the end-points (sites) of the bonds which are present
*/
class BondGraph : public Graph
{
    public :
        vector< pair< pair<int, int>, pair<int, int> > > Bonds;
        /*! \brief Initializes a default BondGraph
        */
        BondGraph();
        /*! \brief Initializes a BondGraph with the given parameters
        \param Bonds The list of the pairs of coordinates of the bonds present in the graph
        \param Identifier The unique number identifying the graph
        \param Order The number of bonds in the graph
        \param LatticeConstant The number of distinct ways to embed this graph in the lattice
        \param SubgraphList The list of subgraphs of this graph, and the number of ways each can be embedded in the graph
        */
        BondGraph(vector< pair< pair<int, int>, pair<int,int> > > & , int, int, int, vector< pair<int, int> > & );
        /*! \brief Adds a bond to the list of bonds
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        */
        void AddBond(pair<int , int> , pair< int, int> );
        /*! \brief Removes a bond from the list of bonds
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        */
        void RemoveBond(pair<int , int> , pair< int, int> );
        /*! \brief Determines whether a bond belongs to the list of bonds
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        */
        bool CheckForBond(pair<int , int> , pair< int, int> );
        /*! \brief Finds how many bonds are adjacent (share a site) with the current bond
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        */
        int BondCount(pair<int , int> , pair< int, int> );
        /*! \brief Transforms the graph into its canonical representation
        Uses the Dihedral class to check all representations of the graph, attemping to maximise (xCoordinates)*Order + yCoordinates. Whichever representation maximises this number is unique and canonical.
        */
        void MakeCanonical();
        /*! \brief Dumps the descriptors of the graph to standard out
        */
        void PrintGraph();
        /*! \brief Creates a one-integer label for each site and determines which sites are connected 
        Assigns each site a label, moving east first, then north. Then determines whether the sites directly east and north of the current site are in the graph and connected to it by an element of Bonds, and if so, notes that each site is connected. 
        */
        void GenerateAdjacencyList();
        /*! \brief Determines the number of distinct ways to embed the graph in the lattice
        Applies each element of the relevant dihedral group to the graph, and determines how many of the results are distinct from each other (there is no way to add an integer to all coordinates and get the other graph).
        */
        void FindLatticeConstant();
        bool operator==(const BondGraph & );
        BondGraph & operator=(const BondGraph &);
};

/*! \class Dihedral
\brief A class for functors describing the operations of the dihedral group of a lattice

Applies a transformation to a list of sites or bonds, rotating or reflecting it. The possible transformations are all elements of the lattice's dihedral group, which describes its symmetries. 
*/
class Dihedral
{
    public :
        int element;
        /*! Sets element to the default (0) which is the identity transformation
        */
        Dihedral();
        /*! Sets element to an index describing which transformation to apply
        \param Factor The index which selects the transformation
        */
        Dihedral(int);
        /*! Applies the transformation to a site
        \param Coordinates The coordinates of the site to be transformed
        */
        void operator() (pair<int,int> & );
        /*! Applies the transformation to a bond
        \param Coordinates The coordinates of the end-points of the bond to be transformed
        */
        void operator() (pair< pair<int,int>, pair<int,int> > & );
} Transform;

/*! \brief Creates all distinct site-based graphs up to some number of sites

Starting with a list of sites, the function adds sites north and east, eliminating raphs which are isomorphic to each other. This continues until all graphs of the maximum order are enumerated.

\param Graphs A collection of graphs to start from, and to which the generated graphs will be appended
\param FinalOrder The maximum order of the graphs which will be generated
*/ 
void ConstructSiteBasedGraphs(vector< vector< SiteGraph > > & , int );


/*! \brief Creates all distinct bond-based graphs up to some number of bonds

Starting with a list of sites, the function adds bonds north and east, eliminating raphs which are isomorphic to each other. This continues until all graphs of the maximum order are enumerated.

\param Graphs A collection of graphs to start from, and to which the generated graphs will be appended
\param FinalOrder The maximum order of the graphs which will be generated
*/ 
void ConstructBondBasedGraphs(vector< vector< BondGraph > > & , int );

/*! \brief Creates all closed rectangular site-based graphs up to some maximum width and height

\param Graphs An empty array which will store the rectangular graphs
\param FinalWidth The maximum width of the rectangular graphs
\param FinalHeight The maximum height of the rectangular graphs
*/
void ConstructRectangularSiteGraphs( vector< vector< SiteGraph > > & , unsigned int , unsigned int );
/*! \brief Creates all closed rectangular site-based graphs up to some maximum order

\param Graphs An empty array which will store the rectangular graphs
\param FinalOrder The maximum order (height * width) of the rectangles
*/
void ConstructRectangularSiteGraphs( vector< vector< SiteGraph > > & , unsigned int );

/*! \brief For each graph in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
*/ 
void FindSubgraphs(vector< vector< SiteGraph > > & );
/*! \brief For each graph in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
*/ 
void FindSubgraphs(vector< SiteGraph > & ); 

//void ReadGraphsFromFile(vector< Graph > & GraphList, const string & file);
void WriteGraphsToFile(vector< SiteGraph > & , string );
void WriteGraphsToFile(vector< vector< SiteGraph > > & , string );
void WriteGraphsToFile(vector< BondGraph > & , string );
void WriteGraphsToFile(vector< vector< BondGraph > > & GraphList, string file);
