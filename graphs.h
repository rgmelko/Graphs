#include <fstream>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "omp.h"

//! \file graphs.h The main header file for the Graphs project

using namespace std;

/*! \class Graph

\brief A superclass containing information about any type of graph

Defines parameters describing a graph without reference to the "type" (site- or bond-based) of the graph, including number of distinct embeddings, number of elements, a unique identifier, the connectivity of elements, and a list of subgraphs. 

\sa SiteGraph, BondGraph
*/

class Graph 
{

    public :
        //! The number of distinct (nonsuperimposable on each other) ways the graph can be embedded in the lattice
        int LatticeConstant;
        //! The number of elements (sites or bonds) in the graph
        int Order;
        //! A unique index to reference the graph with
        int Identifier;
        //! A flag to show whether a graph is site- or bond-based
        bool LowField;
        //! A vector containing pairs of indices, showing which sites in the graph are connected (each site has a unique index)
        std::vector< std::pair<int, int> > AdjacencyList;
        //! A vector containing pairs of indices, listing the Identifiers of graphs which are embeddable in this graph and how many ways this is possible
        std::vector< std::pair<int, int> > SubgraphList;

        /*! \brief Initializes a default graph object
        */
        Graph();
        /*! \brief Initializes a general graph object with the given parameters
        \param LatticeConst The number of distinct ways to embed this graph in the lattice
        \param Order The number of elements in the graph
        \param IdentNumber The unique number which identifies this specific graph
        \param Subgraphs A list of the subgraphs of the graph, and the number of times each subgraph is embeddable in the graph
        */
        Graph(int, int, int, std::vector< std::pair<int, int> > & );
        /*! \brief Initializes a general graph object with the given parameters
        \param LatticeConst The number of distinct ways to embed this graph in the lattice
        \param Order The number of elements in the graph
        \param IdentNumber The unique number which identifies this specific graph
        \param Adjacency A list of which sites in the graph are connected
        \param Subgraphs A list of the subgraphs of the graph, and the number of times each subgraph is embeddable in the graph
        */

        Graph(int, int, int, std::vector< std::pair<int, int> > & , std::vector< std::pair<int, int> > & Subgraphs );
        /*! \brief Assigns the value of one Graph to another
        \param Other A reference to the graph which is being copied
        \returns A reference to a new graph, which is a copy of the referenced graph
        */
        Graph& operator=(const Graph & );
        /*! Determines whether two Graphs have the same descriptors
        \param Other A reference to the graph which is being compared
        \returns A boolean - true if the descriptors are the same, false otherwise 
        */
        bool operator==(const Graph & );
        /*! Counts how many bonds a site has
        \param Site The index of the site to check
        */
        int Valency( int Site);

};

/*! \class SiteGraph
\brief A sublcass of Graph which describes site-based graphs

A site-based graph is one where the connections between sites are less important than the presence of the site at all. The graph is fully described by which sites are or are not present. If two sites are adjacent and present, they must be connected. This class has one additional member: Sites, which describes the real-space locations of the sites which are present
*/
class SiteGraph : public Graph
{
    public :       
        //! A vector containing the real-space coordinates of all the sites present in the graph
        std::vector< std::pair<int, int> > Sites;
        
        /*! \brief Initializes a default SiteGraph
        */
        SiteGraph();
        /*! \brief Initializes a SiteGraph with the given parameters
        \param SiteList The list of the coordinates of the sites present in the graph
        \param IdentNumber The unique number identifying the graph
        \param SiteCount The number of sites in the graph
        \param LatticeConst The number of distinct ways to embed this graph in the lattice
        \param Subgraphs The list of subgraphs of this graph, and the number of ways each can be embedded in the graph
        */
        SiteGraph(std::vector< std::pair<int, int> > & , int, int, int, std::vector< std::pair<int, int> > & );
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
        \return True if the site is present, false otherwise
        */
        bool CheckForSite(int , int );
        /*! \brief Finds the number of sites adjacent to this site in the list of sites
        \param xIndex The x coordinate of the site
        \param yIndex The y coordinate of the site
        \return The number of sites next to the input site
        */
        int SiteDegree(int , int );
        /*! \brief Transforms the graph into its canonical representation
        
        Uses the \link Dihedral Dihedral \endlink class to check all representations of the graph, attemping to find the lexicographically largest. Whichever representation satisfies this condition is unique and canonical.
        First the function creates a temporary storage vector to contain the lexicographically largest representation so far
        \snippet graphs.cpp Creating site canonical storage
        
        The the function checks each possible representation of the graph, grabbing the good copy of the site list and using the \link Dihedral Dihedral \endlink class to transform it.
        
        \snippet graphs.cpp Transforming site canonical with Dihedral

        Each representation is shifted so that their bottom left site is located at (0,0) to make sure that the lexicographic comparison picks the canonical representation.
        
        \snippet graphs.cpp Shifting site canonical

        Finally, the function copies the canonical representation back to the Graph
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
        The function needs to figure out how many representations of the graph cannot be superimposed on each other. There is at least one of these (the canonical representation), so that is copied first. 
        
        \snippet graphs.cpp Site lattice constant copy
    
        The site list is transformed using \link Dihedral Dihedral \endlink to create all possible representations.
        
        \snippet graphs.cpp Site lattice constant dihedral transform
        
        Now the function needs to check if this new representation is superimposable over any of the distinct ones it's already found. It goes through those lists of sites, checking to see if there is a constant shift factor which works for all of them.
        
        \snippet graphs.cpp Site lattice constant shift check
        
        If the new representation is not superimposable over any of the stored ones, it is unique and is stored.
        
        \snippet graphs.cpp Site lattice constant new rep
        
        The number of distinct representations is the lattice constant of the graph.
        */
        void FindLatticeConstant();
        /*! \brief Checks for isomorphism between two site-based graphs
        \param Other The sitegraph to check against
        \return True if the graphs are isomorphic, false otherwise

        Uses the \link Dihedral Dihedral \endlink class to check if Other is isomorphic to the current graph. A graph A is isomorphic to a graph B if it has the same real-space representation (in this case, through the coordinates of its sites) as one of distinct representations of B. A distinct represention of B is any representation of it which can be generated by applying (in this case) the dihedral group of the lattice to B and is not superimposable over any other representation. The operator does this by first, copying the sites representation of the second graph into temporary storage so that a good copy always exists. Then, the function uses a \link Dihedral Dihedral \endlink functor to transform this site representation into one of the eight possible representations. 
        
        \snippet graphs.cpp Site isomorphism copy and transform
        
        The function finds out how far apart the bottom left sites of the reference graph and this representation are, and determines whether all corresponding sites in the two graphs are this far apart. If so, the two graphs are isomorphic.
        
        \snippet graphs.cpp Site isomorphism shift check 

        */ 
        bool operator==(const SiteGraph & );
        /*! \brief Assigns the value of one SiteGraph to another
        \param Other A reference to the site-based graph which is being copied
        \returns A reference to a new site-based graph, which is a copy of the referenced graph
        */
        SiteGraph & operator=(const SiteGraph & );
};

/*! \class BondGraph
\brief A subclass of Graph which describes bond-based graphs

A bond-based graph is one where the connections between sites are more important than the presence of the sites themselves. The graph is fully described by which sites are or are not connected. If two sites are adjacent and present, they may or may not be connected. This class has one additional member: Bonds, which describes the real-space locations of the end-points (sites) of the bonds which are present
*/
class BondGraph : public Graph
{
    public :
        //! A vector which lists all bonds present in the graph by pairing their endpoints (sites in real-space). 
        std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > Bonds;
        
        /*! \brief Initializes a default BondGraph
        */
        BondGraph();
        /*! \brief Initializes a BondGraph with the given parameters
        \param BondList The list of the pairs of coordinates of the bonds present in the graph
        \param IdentNumber The unique number identifying the graph
        \param Order The number of bonds in the graph
        \param LatticeConst The number of distinct ways to embed this graph in the lattice
        \param Subgraphs The list of subgraphs of this graph, and the number of ways each can be embedded in the graph
        */
        BondGraph(std::vector< std::pair< std::pair<int, int>, std::pair<int,int> > > & , int, int, int, std::vector< std::pair<int, int> > & );
        /*! \brief Adds a bond to the list of bonds
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        */
        void AddBond(std::pair<int , int> , std::pair< int, int> );
        /*! \brief Removes a bond from the list of bonds
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        */
        void RemoveBond(std::pair<int , int> , std::pair< int, int> );
        /*! \brief Determines whether a bond belongs to the list of bonds
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        \return True if the bond is present, false otherwise
        */
        bool CheckForBond(std::pair<int , int> , std::pair< int, int> );
        /*! \brief Finds how many bonds are adjacent (share a site) with the current bond
        \param FirstSite The coordinates of the first end-point
        \param SecondSite The coordinates of the second end-point
        \return The number of bonds next to the current bond (for the square lattice, always fewer than 6).
        */
        int BondCount(std::pair<int , int> , std::pair< int, int> );
        /*! \brief Transforms the graph into its canonical representation
        
        Uses the Dihedral class to check all representations of the graph, attemping to maximise (xCoordinates)*Order + yCoordinates. Whichever representation maximises this number is unique and canonical.
        
        First the function creates a temporary storage vector to contain the canonical representation so far
        
        \snippet graphs.cpp Bond canonical storage

        The the function checks each possible representation of the graph, grabbing the good copy of the site list and using the \link Dihedral Dihedral \endlink class to transform it.
        
        \snippet graphs.cpp Bond canonical transform

        Each representation is shifted so that their bottom left site is located at (0,0) to make sure that the comparison picks the canonical representation.
        
        \snippet graphs.cpp Bond canonical shift
        
        */
        void MakeCanonical();
        /*! \brief Dumps the descriptors of the graph to standard out
        */
        void PrintGraph();
        /*! \brief Creates a one-integer label for each site and determines which sites are connected 

        Assigns each site a label, moving east first, then north. Then determines whether the sites directly east and north of the current site are in the graph and connected to it by an element of Bonds, and if so, notes that each site is connected.
        First, the sites (the bond endpoints) are copied and duplicates are removed using std::unique. 
        
        \snippet graphs.cpp Bond adjacency copy
    
        Then the bonds list is traversed again, inserting an element into the AdjacencyList for every bond. std::find searches for the position of the end points in the SiteList.

        \snippet graphs.cpp Bond adjacency fill list
        
        */
        void GenerateAdjacencyList();
        /*! \brief Determines the number of distinct ways to embed the graph in the lattice

        Applies each element of the relevant dihedral group to the graph, and determines how many of the results are distinct from each other (there is no way to add an integer to all coordinates and get the other graph).
        The function needs to figure out how many representations of the graph cannot be superimposed on each other. There is at least one of these (the canonical representation), so that is copied first. 
        
        \snippet graphs.cpp Bond lattice constant copy

        The bond list is transformed using \link Dihedral Dihedral \endlink to create all possible representations. The bond endpoints may end up out of order during transformation, so they are fixed. We only want bonds that point right and up.
        
        \snippet graphs.cpp Bond lattice constant transform
        
        Now the function needs to check if this new representation is superimposable over any of the distinct ones it's already found. It goes through those lists of bonds, checking to see if there is a constant shift factor which works for all of them.
        
        \snippet graphs.cpp Bond lattice constant shift check
        
        If the new representation is not superimposable over any of the stored ones, it is unique and is stored.
        
        \snippet graphs.cpp Bond lattice constant store
        
        The number of distinct representations is the lattice constant of the graph.
        */
        void FindLatticeConstant();
        /*! \brief Counts the number of sites present in a bond list
        \return The total number of sites contained in the graph
        
        Determines the number of distinct sites present as end-points of bonds.

        */
        int NumberSites();
        /*! \brief Checks for isomorphism between two bond-based graphs
        \param Other The bondgraph to check against
        \return True if the graphs are isomorphic, false if otherwise

        Uses the Dihedral class to check if Other is isomorphic to the current graph. A graph A is isomorphic to a graph B if it has the same real-space representation (in this case, through the coordinates of the endpoints of its bonds) as one of distinct representations of B. A distinct represention of B is any representation of it which can be generated by applying (in this case) the dihedral group of the lattice to B and is not superimposable over any other representation. The operator does this by first copying the sites representation of the second graph into temporary storage so that a good copy always exists. Then, the function uses a \link Dihedral Dihedral \endlink functor to transform this bond representation into one of the eight possible representations. It fixes the ordering of the bond endpoints in the case that they've become reversed (bonds should always point up and to the right).  
        
        \snippet graphs.cpp Bond isomorphism copy and transform
        
        The function creates two shifts - one for the first endpoint, one for the second. If they match, the function checks all the bonds to make sure the shift factor is constant. If so, the two graphs are isomorphic. If not, continue until there are no more representations left.  
        
        \snippet graphs.cpp Bond isomorphism shift check

        */ 
        bool operator==(const BondGraph & );
        /*! \brief Assigns the value of one BondGraph to another
        \param Other A reference to the bond-based graph which is being copied
        \returns A reference to a new bond-based graph, which is a copy of the referenced graph
        */
        BondGraph & operator=(const BondGraph &);
};

/*! \class Dihedral
\brief A class for functors describing the operations of the dihedral group of a lattice

Applies a transformation to a list of sites or bonds, rotating or reflecting it. The possible transformations are all elements of the lattice's dihedral group, which describes its symmetries. 
*/
class Dihedral
{
    public :
        //!Which element of the dihedral group to use for the transformation
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
        void operator() (std::pair<int,int> & );
        /*! Applies the transformation to a bond
        \param Coordinates The coordinates of the end-points of the bond to be transformed
        */
        void operator() (std::pair< std::pair<int,int>, std::pair<int,int> > & );
}; //Transform;

/*! 

\fn void ConstructSiteBasedGraphs(std::vector< std::vector< SiteGraph > > & graphs, int FinalOrder)

\brief Creates all distinct site-based graphs up to some number of sites

\param graphs A collection of graphs to start from, and to which the generated graphs will be appended
\param FinalOrder The maximum order of the graphs which will be generated

Starting with a list of sites, the function adds sites north and east, eliminating raphs which are isomorphic to each other. This continues until all graphs of the maximum order are enumerated.

First, the highest order of graphs present in the list is grabbed and the identifying number to start from is grabbed as well. Temporary storage for the new graphs is allocated as well. 

\snippet graphs.cpp Site grab back info

For each order up until the last one, each old graph is copied (so that a good copy remains) and storage for a new graph and new site is created as well. 

\snippet graphs.cpp Site get graph ready

For each site, there are four possible positions to put a new site (north, east, south, west). For each possibility, the new site is generated, and then the function checks to see if it's already present in the site list. If not, it's a valid addition, and it's added to create a new site-based graph. The canonical form of the new graph is found and its adjacency list is generated so that we can see whether this new graph has already been generated.

\snippet graphs.cpp Site make new graph 

Now the function checks to make sure that this graph hasn't already been created, by comparing the site lists (this works because all graphs are in their unique canonical form). If the graph isn't already present, the identifier count is iterated and the graph is added into the list of all graphs.

\snippet graphs.cpp Site check new graph

*/ 

void ConstructSiteBasedGraphs(std::vector< std::vector< SiteGraph > > & , int );

/*! 

\fn void ConstructBondBasedGraphs(std::vector< std::vector< BondGraph > > & graphs, int FinalOrder)

\brief Creates all distinct bond-based graphs up to some number of bonds

\param graphs A collection of graphs to start from, and to which the generated graphs will be appended
\param FinalOrder The maximum order of the graphs which will be generated

Starting with a list of sites, the function adds bonds north and east, eliminating graphs which are isomorphic to each other. This continues until all graphs of the maximum order are enumerated.

First, the highest order of graphs present in the list is grabbed and the identifying number to start from is grabbed as well. Temporary storage for the new graphs is allocated as well. 

\snippet graphs.cpp Bond grab back info

For each order up until the last one, each old graph is copied (so that a good copy remains) and storage for a new graph, new bond, and new site is created as well. 

\snippet graphs.cpp Bond get graph ready

For each site, there are four possible positions to put a new site (north, east, south, west). For each possibility, the new site is generated, and then the function checks to see if it's already present in the site list. If not, it's a valid addition, and it's added to create a new site-based graph. The canonical form of the new graph is found and its adjacency list is generated so that we can see whether this new graph has already been generated.

\snippet graphs.cpp Site make new graph 

Now the function checks to make sure that this graph hasn't already been created, by comparing the site lists (this works because all graphs are in their unique canonical form). If the graph isn't already present, the identifier count is iterated and the graph is added into the list of all graphs.

\snippet graphs.cpp Site check new graph

*/ 
void ConstructBondBasedGraphs(std::vector< std::vector< BondGraph > > & , int );

/*! 
\fn void ConstructRectangularSiteGraphs(std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalWidth, unsigned int FinalHeight)

\brief Creates all closed rectangular site-based graphs up to some maximum width and height

\param graphs An empty array which will store the rectangular graphs
\param FinalWidth The maximum width of the rectangular graphs
\param FinalHeight The maximum height of the rectangular graphs
*/
void ConstructRectangularSiteGraphs( std::vector< std::vector< SiteGraph > > & , unsigned int , unsigned int );

/*! 
\fn void ConstructRectangularSiteGraphs(std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalOrder)

\brief Creates all closed rectangular site-based graphs up to some maximum order

\param graphs An empty array which will store the rectangular graphs
\param FinalOrder The maximum order (height * width) of the rectangles
*/
void ConstructRectangularSiteGraphs( std::vector< std::vector< SiteGraph > > & , unsigned int );

/*! 
\fn void FindSubgraphs(std::vector< std::vector< SiteGraph > > & GraphList)

\brief For each graph in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
*/ 
void FindSubgraphs(std::vector< std::vector< SiteGraph > > & );

/*! 
\fn void FindSubgraphs(std::vector< std::vector< SiteGraph > > & GraphList, unsigned int Index)

\brief For each graph of a certain order in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
\param Index The order of the subset of graphs to find subgraphs for
*/ 
void FindSubgraphs(std::vector< std::vector< SiteGraph > > & , unsigned int );

/*! 
\fn void FindSubgraphs(std::vector< SiteGraph > & GraphList)

\brief For each graph in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
*/ 
void FindSubgraphs(std::vector< SiteGraph > & ); 

/*! 
\fn void FindSubgraphs(vector< vector< BondGraph > > & GraphList)

\brief For each graph in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
*/ 
void FindSubgraphs(std::vector< std::vector< BondGraph > > & );

/*!
\fn void FindSubgraphs(vector< vector< BondGraph > > & GraphList, unsigned int Index)

\brief For each graph of a certain order in a list, finds all other graphs in the list which are its subgraphs, and how many ways there are to embed each subgraph in the graph

\param GraphList The list of graphs to extract subgraphs from
\param Index The order of the subset of graphs to find subgraphs for
*/ 
void FindSubgraphs(std::vector< std::vector< BondGraph > > & , unsigned int );

/*! 
\fn void ReadGraphsFromFile( vector< Graph > & GraphList, const string & File)
\brief Fills a vector with graphs from a file on disk

\param GraphList The empty vector which will be filled with the graphs from file
\param File The location of the file containing the graphs to be read in
*/
void ReadGraphsFromFile(std::vector< Graph > & , const string & );

/*! 
\fn void WriteGraphsToFile(std::vector<SiteGraph> & GraphList, string File)
\brief Writes graphs from a vector a file on disk

\param GraphList The vector of site-based graphs to write to file
\param File The location of the file to write the graphs to
*/
void WriteGraphsToFile(std::vector< SiteGraph > & , string );
/*! 
\fn void WriteGraphsToFile(std::vector< std::vector<SiteGraph> > & GraphList, string File)
\brief Writes graphs from a vector a file on disk

\param GraphList The vector of site-based graphs to write to file
\param File The location of the file to write the graphs to
*/
void WriteGraphsToFile(std::vector< std::vector< SiteGraph > > & , string );

/*! 
\fn void WriteGraphsToFile(std::vector< std::vector<SiteGraph> > & GraphList, string File, unsigned int Index)
\brief Writes a subset of graphs from a vector a file on disk

\param GraphList The vector of site-based graphs to write to file
\param File The location of the file to write the graphs to
\param Index The index of the vector (in the vector-of-vectors) containing the subset of graphs to be written
*/
void WriteGraphsToFile(std::vector< std::vector< SiteGraph > > & , string , unsigned int );

/*! 
\fn void WriteGraphsToFile(std::vector<BondGraph> & GraphList, string File)
\brief Writes graphs from a vector a file on disk

\param GraphList The vector of site-based graphs to write to file
\param File The location of the file to write the graphs to
*/
void WriteGraphsToFile(std::vector< BondGraph > & , string );

/*! 
\fn void WriteGraphsToFile(std::vector< std::vector<BondGraph> > & GraphList, string File)
\brief Writes graphs from a vector a file on disk

\param GraphList The vector of site-based graphs to write to file
\param File The location of the file to write the graphs to
*/
void WriteGraphsToFile(std::vector< std::vector< BondGraph > > & , string );

/*! 
\fn void WriteGraphsToFile(std::vector< std::vector<BondGraph> > & GraphList, string File, unsigned int Index)
\brief Writes a subset of graphs from a vector a file on disk

\param GraphList The vector of site-based graphs to write to file
\param File The location of the file to write the graphs to
\param Index The index of the vector (in the vector-of-vectors) containing the subset of graphs to be written
*/
void WriteGraphsToFile(std::vector< std::vector< BondGraph > > & , string , unsigned int );
