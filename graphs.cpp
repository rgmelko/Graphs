#include "graphs.h"

//! \file graphs.cpp The main source file for the Graphs project

/*!
\author Katharine Hyatt
\author Roger Melko
\author Ann Kallin
*/

/*!
\mainpage 

This software is intended to be used to generate graphs for use in numerical physics simulations. It's designed to generate all site- or bond-based graphs up to some order which are embeddable in a particular two dimensional lattice, such as the square lattice, honeycomb lattice, or Kagome lattice. This documentation describes the \link Graph Graph \endlink class, which is used to represent and manipulate graphs, as well as the \link Dihedral Dihedral \endlink class, which is used to represent the symmetry relations of the lattice and operate on these graphs. It also describes functions for automatically generating graphs, finding their subgraphs, and performing I/O on them. This software is licensed under the MIT license, available in the LICENSE file in the git repo. 

*/

int main()
{
    //vector< vector< SiteGraph > > rectangles;
    //ConstructRectangularSiteGraphs(rectangles, 2, 3);
    vector< vector< SiteGraph > > testsites;
    testsites.resize(1);
    std::vector< std::pair<int,int> > SiteList;
    SiteList.resize(1);
    SiteList[0].first = 0;
    SiteList[0].second = 0;
    std::vector< std::pair<int,int> > Empty;
    SiteGraph Start(SiteList, 0, 1, 1, Square, Empty); 
    testsites[0].resize(1);
    testsites[0][0] = Start;
    for(unsigned int i = 1; i <= 4; i++)
    {
        ConstructSiteBasedGraphs(testsites, i);
        FindSubgraphs(testsites, i);
        WriteGraphsToFile(testsites, "4square.dat", i);
    }
    /*
    vector< vector< BondGraph > > testbonds;
    testbonds.resize(2);
    vector<std::pair< std::pair<int,int>, std::pair<int,int> > > BondList1;
    vector<std::pair< std::pair<int,int>, std::pair<int,int> > > BondList2;
    BondList1.resize(1);
    BondList2.resize(1);
    BondList1[0].first.first = 0;
    BondList1[0].first.second = 0;
    BondList1[0].second.first = 0;
    BondList1[0].second.second = 0;
    BondList2[0].first.first = 0;
    BondList2[0].first.second = 0;
    BondList2[0].second.first = 1;
    BondList2[0].second.second = 0;
    vector< std::pair<int,int> > Empty1;
    vector< std::pair<int,int> > Empty2;
    BondGraph Start1(BondList1, 0, 0, 1, Square, Empty1); 
    BondGraph Start2(BondList2, 1, 1, 2, Square, Empty2);
    testbonds[0].resize(1);
    testbonds[0][0] = Start1;
    testbonds[0][0].LowField = true;
    testbonds[1].resize(1);
    testbonds[1][0] = Start2;
    testbonds[1][0].Identifier = 1;
    testbonds[1][0].LowField = true;
    testbonds[1][0].GenerateAdjacencyList();
    ConstructBondBasedGraphs(testbonds, 4);
    FindSubgraphs(testbonds);
    WriteGraphsToFile(testbonds, "test4bondbased.dat");
    */
    return 0;

}

Graph::Graph()
{
    Order           = 0;
    LatticeConstant = 1;
    LatticeType     = Square;
    Identifier      = 0;
    SubgraphList.clear();
}

Graph::Graph(int IdentNumber, int ElementCount, int LatticeConst, Geometry Type, std::vector< std::pair<int, int> > & Subgraphs )
{
    Identifier      = IdentNumber;
    Order           = ElementCount;
    LatticeConstant = LatticeConst;
    LatticeType     = Type;
    SubgraphList    = Subgraphs;
}

Graph::Graph(int IdentNumber, int ElementCount, int LatticeConst, Geometry Type, std::vector< std::pair<int, int> > & Adjacency, std::vector< std::pair<int, int> > & Subgraphs )
{
    Identifier      = IdentNumber;
    Order           = ElementCount;
    LatticeConstant = LatticeConst;
    LatticeType     = Type;
    AdjacencyList   = Adjacency;
    SubgraphList    = Subgraphs;
}

Graph& Graph::operator=( const Graph & Other)
{
    this->Order           = Other.Order;
    this->LatticeConstant = Other.LatticeConstant;
    this->LatticeType     = Other.LatticeType;
    this->SubgraphList    = Other.SubgraphList;
    this->LowField        = Other.LowField;
    return *this;
}

bool Graph::operator==( const Graph & Other)
{
    return (( this->Order           == Other.Order) &&
           (  this->LatticeConstant == Other.LatticeConstant) &&
           (  this->LatticeType     == Other.LatticeType) );
}

int Graph::Valency( int Site )
{
    int Count = 0;
    for( unsigned int CurrentConnection = 0; CurrentConnection < this->AdjacencyList.size(); CurrentConnection++ )
    {
        if ( this->AdjacencyList[ CurrentConnection ].first  == Site || 
             this->AdjacencyList[ CurrentConnection ].second == Site )
        {
            ++Count;
        }
    }
    return Count;
}

int Graph::DihedralSize()
{
    switch( this->LatticeType )
    {
        case 0 :
            return 8;
        case 1 :
            return 12;
        case 2 :
            return 12;
        case 3 :
            return 12;
        default :
            return 8;
    }    
} 

SiteGraph::SiteGraph()
{
    Order           = 0;
    LatticeConstant = 1;
    Identifier      = 0;
    LatticeType     = Square;
    Sites.clear();
    SubgraphList.clear();
}

SiteGraph::SiteGraph( 
    std::vector< std::pair< int, int > > & SiteList, 
    int IdentNumber, 
    int SiteCount, 
    int LatticeConst, 
    Geometry Type, 
    std::vector< std::pair<int, int> > & Subgraphs )
{
    Identifier      = IdentNumber;
    Order           = SiteCount;
    LatticeConstant = LatticeConst;
    LatticeType     = Type;
    SubgraphList    = Subgraphs;
    Sites           = SiteList;
}

SiteGraph& SiteGraph::operator=( const SiteGraph & Other )
{
    this->Order           = Other.Order;
    this->LatticeConstant = Other.LatticeConstant;
    this->LatticeType     = Other.LatticeType;
    this->SubgraphList    = Other.SubgraphList;
    this->Sites           = Other.Sites;
    this->LowField        = Other.LowField;
    return *this;
}

void SiteGraph::AddSite( int xIndex, int yIndex )
{
    unsigned int InsertCounter = 0;
    while (  InsertCounter < this->Sites.size() && this->Sites[ InsertCounter ].first <= xIndex && this->Sites[ InsertCounter ].second <= yIndex )
    {
        InsertCounter++;
    }
    this->Sites.insert( this->Sites.begin() + InsertCounter, std::make_pair( xIndex, yIndex ) );
}

void SiteGraph::RemoveSite( int xIndex, int yIndex )
{
    unsigned int EraseCounter = 0;
    while ( this->Sites[ EraseCounter ].first <= xIndex && this->Sites[ EraseCounter ].second <= yIndex && EraseCounter< this->Sites.size() )
    {
        EraseCounter++;
    }
    this->Sites.erase( this->Sites.begin() + EraseCounter );
}

bool SiteGraph::CheckForSite( int xIndex, int yIndex )
{
    std::pair< int, int> TempSite = std::make_pair( xIndex, yIndex ); 

    return std::binary_search( this->Sites.begin(), this->Sites.end(), TempSite );
}

void SiteGraph::PrintGraph()
{
    cout<<"ID number: "<<this->Identifier<<endl;
    cout<<"Order: "<<this->Order<<endl;
    cout<<"Lattice Constant: "<<this->LatticeConstant<<endl;
    cout<<"Real space coordinates: "<<endl;
    for( unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++ )
    {
        cout<<"("<<this->Sites[ CurrentSite ].first<<", "<<this->Sites[ CurrentSite ].second<<") ";
    }
    cout<<endl;
    cout<<"Subgraph List: "<<endl;
    for ( unsigned int CurrentSubgraph = 0; CurrentSubgraph < this->SubgraphList.size(); CurrentSubgraph++ )
    {
        cout<<"("<<this->SubgraphList[ CurrentSubgraph ].first<<", "<<this->SubgraphList[ CurrentSubgraph ].second<<") ";
    }
    cout<<endl;
    cout<<"Adjacency List: "<<endl;
    for ( unsigned int CurrentBond = 0; CurrentBond < this->AdjacencyList.size(); CurrentBond++ )
    {
        cout<<"("<<this->AdjacencyList[ CurrentBond ].first<<", "<<this->AdjacencyList[ CurrentBond ].second<<") ";
    }
    cout<<endl;
}

void SiteGraph::FindLatticeConstant()
{
    std::vector< std::vector< std::pair< int, int > > > SiteLists;
   
    this->GenerateDistinctReps( SiteLists );
    this->LatticeConstant = (int)SiteLists.size();
}

void SiteGraph::GenerateDistinctReps( std::vector< std::vector< std::pair< int, int > > > & Representations )
{

    Representations.clear();
    Representations.push_back( this->Sites );

    int MaxElement = this->DihedralSize();
    for( int CurrentElement = 1; CurrentElement < MaxElement; CurrentElement++)
    {
        Dihedral Transform( CurrentElement, this->LatticeType );
        std::vector< std::pair< int, int > > ThisRep = this->Sites;
        std::for_each( ThisRep.begin(), ThisRep.end(), Transform );
        std::sort( ThisRep.begin(), ThisRep.end() );
        bool GlobalShifted = false;
        for ( unsigned int CurrentRep = 0; CurrentRep < Representations.size(); CurrentRep++ )
        {
            bool Shifted = true;
            const std::pair< int, int > shift = std::make_pair( Representations[ CurrentRep ].front().first - ThisRep.front().first, Representations[ CurrentRep ].front().second - ThisRep.front().second);

            unsigned int CurrentSite = 1;
            while( Shifted && CurrentSite < Representations[ CurrentRep ].size())
            {
                Shifted = Shifted && ( ( shift.first == ( Representations[ CurrentRep ][ CurrentSite ].first - ThisRep[ CurrentSite ].first ) ) && ( shift.second == ( Representations[ CurrentRep ][ CurrentSite ].second - ThisRep[ CurrentSite ].second ) ) );
                CurrentSite++;
            }
    
            GlobalShifted = GlobalShifted || Shifted;
        }
        if ( !GlobalShifted )
        {
            const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
            for( unsigned int CurrentSite = 0; CurrentSite < ThisRep.size(); CurrentSite++)
            {
                ThisRep[ CurrentSite ].first  += shift.first;
                ThisRep[ CurrentSite ].second += shift.second;
            }
            Representations.push_back( ThisRep );
        }
    }
}

Dihedral::Dihedral()
{
    element = 0;
}

Dihedral::Dihedral( int Element, Geometry Order )
{
    element = Element;
    factor  = Order;
}

void Dihedral::operator()( std::pair< int, int > & Coordinates )
{
    int TempFirst;
    int TempSecond;
    switch( this->factor )
    {
        case 0 : //Square
            switch( this->element )
            {
                case 0 :
                    break;
                case 1 :
                    TempFirst          = Coordinates.first;
                    Coordinates.first  = -Coordinates.second;
                    Coordinates.second = TempFirst;
                    break;
                case 2 : 
                    Coordinates.first  = -Coordinates.first;
                    Coordinates.second = -Coordinates.second;
                    break;
                case 3 :
                    TempSecond         = Coordinates.second;
                    Coordinates.second = -Coordinates.first;
                    Coordinates.first  = TempSecond;
                    break;
                case 4 : 
                    Coordinates.first  = -Coordinates.first;
                    break;
                case 5 :
                    Coordinates.second = -Coordinates.second;
                    break;
                case 6 :
                    TempFirst          = -Coordinates.first;
                    TempSecond         = -Coordinates.second;
                    Coordinates.first  = TempSecond;
                    Coordinates.second = TempFirst;
                    break;
                case 7 : 
                    TempFirst          = Coordinates.first;
                    TempSecond         = Coordinates.second;
                    Coordinates.first  = TempSecond;
                    Coordinates.second = TempFirst;
                    break;
            }
            break;

        case 1 : //Triangular
        case 2 : //Kagome
        case 3 : //Honeycomb
            switch( this->element )
            {
                case 0 : // Identity
                    break;
                case 1 : // Rotate counter clockwise by pi/3
                    TempFirst          = Coordinates.first;
                    TempSecond         = Coordinates.second;
                    Coordinates.first  = -TempSecond;
                    Coordinates.second = TempSecond + TempFirst;
                    break;
                case 2 : // Rotate counter clockwise by 2pi/3
                    TempFirst          = Coordinates.first;
                    TempSecond         = Coordinates.second;
                    Coordinates.first  = -TempFirst - TempSecond;
                    Coordinates.second = TempFirst;
                    break;
                case 3 : // Rotate counterclockwise by pi
                    Coordinates.first  = -Coordinates.first;
                    Coordinates.second = -Coordinates.second;
                    break;
                case 4 : // Rotate counterclockwise by 4pi/3
                    TempFirst          = Coordinates.first;
                    TempSecond         = Coordinates.second;
                    Coordinates.first  = TempSecond;
                    Coordinates.second = -TempSecond - TempFirst;
                    break;
                case 5 : // Rotate counterclockwise by 5pi/3
                    Coordinates.first  = -Coordinates.first;
                    Coordinates.second = -Coordinates.second;
                    TempFirst          = Coordinates.first;
                    TempSecond         = Coordinates.second;
                    Coordinates.first  = -TempFirst - TempSecond;
                    Coordinates.second = TempFirst;
                    break;
                case 6 : // Flip along a_1
                    Coordinates.first  = Coordinates.first + Coordinates.second;
                    Coordinates.second = -Coordinates.second;
                    break;
                case 7 : // Flip along a_2
                    Coordinates.second = Coordinates.first + Coordinates.second;
                    Coordinates.first  = -Coordinates.first;
                    break;
                case 8 : // Flip along a_2 - a_1
                    TempFirst          = Coordinates.first;
                    TempSecond         = Coordinates.second;
                    Coordinates.first  = -TempSecond;
                    Coordinates.second = -TempFirst;
                    break;
                case 9 : // Flip along vertical
                    Coordinates.first  = -( Coordinates.first + Coordinates.second );
                    break;
                case 10 : // Flip through one edge
                    TempFirst          = Coordinates.first;
                    Coordinates.first  = Coordinates.second;
                    Coordinates.second = TempFirst;
                    break;
                case 11 : // Flip through remaining edge
                    Coordinates.second = -( Coordinates.first + Coordinates.second );
                    break;
            }
            break;
    
    }
}

void Dihedral::operator()( std::pair< std::pair< int, int > , std::pair< int, int > > & Coordinates )
{
    int TempFirst;
    int TempSecond;
    
    switch( this->factor )
    {
        case 0 : // Square 
            switch( this->element )
            {
                case 0 :
                    break;
                case 1 :
                    TempFirst                 = Coordinates.first.first;
                    Coordinates.first.first   = -Coordinates.first.second;
                    Coordinates.first.second  = TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    Coordinates.second.first  = -Coordinates.second.second;
                    Coordinates.second.second = TempFirst;
                    break;
                case 2 : 
                    Coordinates.first.first   = -Coordinates.first.first;
                    Coordinates.first.second  = -Coordinates.first.second;
                    Coordinates.second.first  = -Coordinates.second.first;
                    Coordinates.second.second = -Coordinates.second.second;
                    break;
                case 3 :
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.second  = -Coordinates.first.first;
                    Coordinates.first.first   = TempSecond;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.second = -Coordinates.second.first;
                    Coordinates.second.first  = TempSecond;
                    break;
                case 4 : 
                    Coordinates.first.first   = -Coordinates.first.first;
                    Coordinates.second.first  = -Coordinates.second.first;
                    break;
                case 5 :
                    Coordinates.first.second  = -Coordinates.first.second;
                    Coordinates.second.second = -Coordinates.second.second;
                    break;
                case 6 :
                    TempFirst                 = -Coordinates.first.first;
                    TempSecond                = -Coordinates.first.second;
                    Coordinates.first.first   = TempSecond;
                    Coordinates.first.second  = TempFirst;
                    TempFirst                 = -Coordinates.second.first;
                    TempSecond                = -Coordinates.second.second;
                    Coordinates.second.first  = TempSecond;
                    Coordinates.second.second = TempFirst;
                    break;
                case 7 : 
                    TempFirst                 = Coordinates.first.first;
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.first   = TempSecond;
                    Coordinates.first.second  = TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.first  = TempSecond;
                    Coordinates.second.second = TempFirst;
                    break;
            }
            break;
        case 1 : //Triangular;
        case 2 : //Kagome
        case 3 : //Honeycomb
            switch( this->element )
            {
                case 0 : // Identity
                    break;
                case 1 : // Rotate counter clockwise by pi/3
                    TempFirst                 = Coordinates.first.first;
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.first   = -TempSecond;
                    Coordinates.first.second  = TempSecond + TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.first  = -TempSecond;
                    Coordinates.second.second = TempSecond + TempFirst;
                    break;
                case 2 : // Rotate counter clockwise by 2pi/3
                    TempFirst                 = Coordinates.first.first;
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.first   = -TempFirst - TempSecond;
                    Coordinates.first.second  = TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.first  = -TempFirst - TempSecond;
                    Coordinates.second.second = TempFirst;
                    break;
                case 3 : // Rotate counterclockwise by pi
                    Coordinates.first.first   = -Coordinates.first.first;
                    Coordinates.first.second  = -Coordinates.first.second;
                    Coordinates.second.first  = -Coordinates.second.first;
                    Coordinates.second.second = -Coordinates.second.second;
                    break;
                case 4 : // Rotate counterclockwise by 4pi/3
                    TempFirst                 = Coordinates.first.first;
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.first   = TempSecond;
                    Coordinates.first.second  = -TempSecond - TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.first  = TempSecond;
                    Coordinates.second.second = -TempSecond - TempFirst;
                    break;
                case 5 : // Rotate counterclockwise by 5pi/3
                    Coordinates.first.first   = -Coordinates.first.first;
                    Coordinates.first.second  = -Coordinates.first.second;
                    TempFirst                 = Coordinates.first.first;
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.first   = -TempFirst - TempSecond;
                    Coordinates.first.second  = TempFirst;
                    Coordinates.second.first  = -Coordinates.second.first;
                    Coordinates.second.second = -Coordinates.second.second;
                    TempFirst                 = Coordinates.second.first;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.first  = -TempFirst - TempSecond;
                    Coordinates.second.second = TempFirst;
                    break;
                case 6 : // Flip along a_1
                    Coordinates.first.first   = Coordinates.first.first + Coordinates.first.second;
                    Coordinates.first.second  = -Coordinates.first.second;
                    Coordinates.second.first  = Coordinates.second.first + Coordinates.second.second;
                    Coordinates.second.second = -Coordinates.second.second;
                    break;
                case 7 : // Flip along a_2
                    Coordinates.first.second  = Coordinates.first.first + Coordinates.first.second;
                    Coordinates.first.first   = -Coordinates.first.first;
                    Coordinates.second.second = Coordinates.second.first + Coordinates.second.second;
                    Coordinates.second.first  = -Coordinates.second.first;
                    break;
                case 8 : // Flip along a_2 - a_1
                    TempFirst                 = Coordinates.first.first;
                    TempSecond                = Coordinates.first.second;
                    Coordinates.first.first   = -TempSecond;
                    Coordinates.first.second  = -TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    TempSecond                = Coordinates.second.second;
                    Coordinates.second.first  = -TempSecond;
                    Coordinates.second.second = -TempFirst;
                    break;
                case 9 : // Flip along vertical
                    Coordinates.first.first   = -( Coordinates.first.first + Coordinates.first.second );
                    Coordinates.second.first  = -( Coordinates.second.first + Coordinates.second.second );
                    break;
                case 10 : // Flip through one edge
                    TempFirst                 = Coordinates.first.first;
                    Coordinates.first.first   = Coordinates.first.second;
                    Coordinates.first.second  = TempFirst;
                    TempFirst                 = Coordinates.second.first;
                    Coordinates.second.first  = Coordinates.second.second;
                    Coordinates.second.second = TempFirst;
                    break;
                case 11 : // Flip through remaining edge
                    Coordinates.first.second  = -( Coordinates.first.first + Coordinates.first.second );
                    Coordinates.second.second = -( Coordinates.second.first + Coordinates.second.second );
                    break;
            }
            break;
    }
}

int SiteGraph::Embeddings( const SiteGraph & Other )
{
    std::vector< std::vector< std::pair< int, int > > > DistinctReps;
    this->GenerateDistinctReps( DistinctReps );
    int EmbeddingsCounter = 0;

    for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
    {
        std::pair< int, int > shift = std::make_pair( 0, 0 );
        int xMax = 0;
        int yMax = 0;
        int xMin = 0;
        int yMin = 0;
        for( unsigned int CurrentElement = 0; CurrentElement < Other.Sites.size(); CurrentElement++ )
        {
            xMax = ( Other.Sites[ CurrentElement ].first > xMax )  ? Other.Sites[ CurrentElement ].first  : xMax;
            yMax = ( Other.Sites[ CurrentElement ].second > yMax ) ? Other.Sites[ CurrentElement ].second : yMax;
            xMin = ( Other.Sites[ CurrentElement ].first < xMin )  ? Other.Sites[ CurrentElement ].first  : xMin;
            yMin = ( Other.Sites[ CurrentElement ].second < yMin ) ? Other.Sites[ CurrentElement ].second : yMin;
        }

        for( int xBoost = xMin; xBoost <= xMax; xBoost++ )
        {
            for( int yBoost = yMin; yBoost <= yMax; yBoost++ )
            {
                shift = std::make_pair( xBoost, yBoost );
                std::vector< std::pair< int, int > > CheckList = DistinctReps[ CurrentRep ];
                unsigned int Counter = 0;
                for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps[ CurrentRep ].size(); CurrentSite++ )
                {
                    CheckList[ CurrentSite ].first  += shift.first;
                    CheckList[ CurrentSite ].second += shift.second;
                    Counter += std::binary_search( Other.Sites.begin(), Other.Sites.end(), CheckList[ CurrentSite ] );
                }
                if ( Counter == CheckList.size() )
                {
                    EmbeddingsCounter++;
                }    
            }
        }
    }
    return EmbeddingsCounter;
}

bool SiteGraph::operator==( const SiteGraph & Other )
{
    if( ( Other.LatticeConstant == this->LatticeConstant ) &&
        ( Other.LatticeType == this->LatticeType ) &&
        ( Other.Order == this->Order) )
    {
        int MaxElement = this->DihedralSize();
        
        for( int currentFactor = 0; currentFactor < MaxElement; currentFactor++ )
        {
            std::vector< std::pair< int, int > > SitesCopy = Other.Sites;
            Dihedral Transform( currentFactor, this->LatticeType );
            std::for_each( SitesCopy.begin(), SitesCopy.end(), Transform );
            std::sort( SitesCopy.begin(), SitesCopy.end() );
            
            bool Isomorphic = true; 
            const std::pair< int, int > shift = std::make_pair( this->Sites.front().first - SitesCopy.front().first, this->Sites.front().second - SitesCopy.front().second );
            
            unsigned int CurrentSite = 0;
            while( Isomorphic && CurrentSite < SitesCopy.size() )
            {
                int ShiftX = ( this->Sites[ CurrentSite ].first - SitesCopy[ CurrentSite ].first );
                int ShiftY = ( this->Sites[ CurrentSite ].second - SitesCopy[ CurrentSite ].second );
                Isomorphic = ( Isomorphic ) && ( shift == std::make_pair( ShiftX, ShiftY ) );
                CurrentSite++;
            }

            if( this->AdjacencyList.size() > 0 && Other.AdjacencyList.size() > 0 )
            {
                Isomorphic = Isomorphic || ( this->AdjacencyList == Other.AdjacencyList );
            }
            if( Isomorphic )
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

int SiteGraph::SiteDegree( int xIndex, int yIndex )
{
    int DegreeCounter = 0;
    for( unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++ )
    {
        if( ( ( this->Sites[ CurrentSite ].first  == xIndex - 1 || 
                this->Sites[ CurrentSite ].first  == xIndex + 1 ) && 
                this->Sites[ CurrentSite ].second == yIndex ) ||
            ( ( this->Sites[ CurrentSite ].second == yIndex - 1 || 
                this->Sites[ CurrentSite ].second == yIndex + 1 ) &&
                this->Sites[ CurrentSite ].first  == xIndex ) )
         {
            DegreeCounter++;
         }
    }
    return DegreeCounter;
}

void SiteGraph::MakeCanonical()
{
    std::vector< std::pair< int, int > > CanonicalSites;
    int MaxElement = this->DihedralSize();
    
    for( int currentFactor = 0; currentFactor < MaxElement; currentFactor++ )
    {
        std::vector< std::pair< int, int > > SitesCopy = this->Sites;

        Dihedral Transform( currentFactor, this->LatticeType );
        std::for_each( SitesCopy.begin(), SitesCopy.end(), Transform );
        std::sort( SitesCopy.begin(), SitesCopy.end() );
        std::pair< int, int > shift = std::make_pair( -SitesCopy.front().first, -SitesCopy.front().second );
        for( unsigned int CurrentSite = 0; CurrentSite < SitesCopy.size(); CurrentSite++ )
        {
            SitesCopy[ CurrentSite ] = std::make_pair( SitesCopy[ CurrentSite ].first + shift.first, SitesCopy[ CurrentSite ].second + shift.second );
        }
        
        if( std::lexicographical_compare( CanonicalSites.begin(), CanonicalSites.end(), SitesCopy.begin(), SitesCopy.end() ) )
        {
            CanonicalSites = SitesCopy;
        }
    }
    this->Sites = CanonicalSites;
}

void SiteGraph::GenerateAdjacencyList()
{
    this->AdjacencyList.clear();
    for( unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++ )
    {
        switch( this->LatticeType )
        {
            case 0 : //Square
                for( unsigned int CurrentIndex = CurrentSite + 1; CurrentIndex < this->Sites.size(); CurrentIndex++ )
                {
                    if( ( Sites[ CurrentIndex ].first  == Sites[ CurrentSite ].first && 
                          Sites[ CurrentIndex ].second == Sites[ CurrentSite ].second + 1 )  ||
                        ( Sites[ CurrentIndex ].second == Sites[ CurrentSite ].second && 
                          Sites[ CurrentIndex ].first  == Sites[ CurrentSite ].first + 1 ) ) 
                    {
                        AdjacencyList.push_back( std::make_pair( CurrentSite, CurrentIndex ) );
                    }
                }
                break;
            case 1 : //Triangular
                for( unsigned int CurrentIndex = CurrentSite + 1; CurrentIndex < this->Sites.size(); CurrentIndex++ )
                {
                    if ( ( ( Sites[ CurrentIndex ].first  == Sites[ CurrentSite ].first && 
                             Sites[ CurrentIndex ].second == Sites[ CurrentSite ].second + 1 )  ||
                           ( Sites[ CurrentIndex ].second == Sites[ CurrentSite ].second && 
                             Sites[ CurrentIndex ].first  == Sites[ CurrentSite ].first + 1 ) ) ||
                           ( Sites[ CurrentIndex ].second == Sites[ CurrentSite ].second - 1 && 
                             Sites[ CurrentIndex ].first  == Sites[ CurrentSite ].first + 1 ) )
                    {
                        AdjacencyList.push_back( std::make_pair( CurrentSite, CurrentIndex ) );
                    }
                }
                break;
            case 2 : //Kagome
            case 3 : //Honeycomb
                break;
        }
    }
}

BondGraph::BondGraph()
{
    Order           = 0;
    LatticeConstant = 1;
    Identifier      = 0;
    LatticeType     = Square;
    Bonds.clear();
    SubgraphList.clear();
}

BondGraph::BondGraph(
    std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > & BondList, 
    int IdentNumber, 
    int BondNumber, 
    int LatticeConst, 
    Geometry Type, 
    std::vector< std::pair< int, int > > & Subgraphs )
{
    Identifier      = IdentNumber;
    Order           = BondNumber;
    LatticeConstant = LatticeConst;
    LatticeType     = Type;
    SubgraphList    = Subgraphs;
    Bonds           = BondList;
}

BondGraph& BondGraph::operator=( const BondGraph & Other )
{
    this->Order           = Other.Order;
    this->LatticeConstant = Other.LatticeConstant;
    this->LatticeType     = Other.LatticeType;
    this->SubgraphList    = Other.SubgraphList;
    this->Bonds           = Other.Bonds;
    this->LowField        = Other.LowField;
    return *this;
}

int BondGraph::Embeddings( const BondGraph & Other )
{
    std::vector< std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > > DistinctReps;
    this->GenerateDistinctReps( DistinctReps );
    //Now we have all distinct permutations of the graph!

    int EmbeddingsCounter = 0;
    for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
    {
        std::pair< int, int > shift = std::make_pair( 0, 0 );
        int xMax = 0;
        int xMin = 0; 
        int yMax = 0;
        int yMin = 0;
        for( unsigned int CurrentElement = 0; CurrentElement < Other.Bonds.size(); CurrentElement++ )
        {
            xMax = ( Other.Bonds[ CurrentElement ].second.first > xMax )  ? Other.Bonds[ CurrentElement ].second.first: xMax;
            yMax = ( Other.Bonds[ CurrentElement ].second.second > yMax ) ? Other.Bonds[ CurrentElement ].second.second : yMax;
            xMin = ( Other.Bonds[ CurrentElement ].first.first < xMin )   ? Other.Bonds[ CurrentElement ].first.first : xMin;
            yMin = ( Other.Bonds[ CurrentElement ].first.second < yMin )  ? Other.Bonds[ CurrentElement ].first.second : yMin;
        }
        for( int xBoost = xMin; xBoost <= xMax; xBoost++ )
        {
            for( int yBoost = yMin; yBoost <= yMax; yBoost++ )
            {
                shift = std::make_pair( xBoost, yBoost );
                std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > CheckList = DistinctReps[ CurrentRep ];
                unsigned int Counter = 0;
                for( unsigned int CurrentBond = 0; CurrentBond < DistinctReps[ CurrentRep ].size(); CurrentBond++ )
                {
                    CheckList[ CurrentBond ].first.first   += shift.first;
                    CheckList[ CurrentBond ].first.second  += shift.second;
                    CheckList[ CurrentBond ].second.first  += shift.first;
                    CheckList[ CurrentBond ].second.second += shift.second;
                    Counter += std::binary_search( Other.Bonds.begin(), Other.Bonds.end(), CheckList[ CurrentBond ] );
                }
                if ( Counter == CheckList.size() )
                {
                    EmbeddingsCounter++;
                }
            }
        }
    }
    return EmbeddingsCounter;
}

bool BondGraph::operator==( const BondGraph & Other )
{
    if( ( Other.Order == this->Order ) && ( Other.LatticeType == this->LatticeType ) )
    {
        int MaxElement = this->DihedralSize();
        
        for( int currentFactor = 0; currentFactor < MaxElement; currentFactor++ )
        {
            std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > BondsCopy = Other.Bonds;
            Dihedral Transform( currentFactor, this->LatticeType );
            std::for_each( BondsCopy.begin(), BondsCopy.end(), Transform );
            for( unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++ )
            {
                std::pair< int, int > Temp = BondsCopy[ CurrentBond ].first;
                if( BondsCopy[ CurrentBond ].first > BondsCopy[ CurrentBond ].second )
                {
                    BondsCopy[ CurrentBond ].first  = BondsCopy[ CurrentBond ].second;
                    BondsCopy[ CurrentBond ].second = Temp;
                }
            }
            std::sort( BondsCopy.begin(), BondsCopy.end() );
            std::pair< int, int > shift1 = std::make_pair( this->Bonds.front().first.first - BondsCopy.front().first.first, this->Bonds.front().first.second - BondsCopy.front().first.second );
            std::pair< int, int > shift2 = std::make_pair( this->Bonds.front().second.first - BondsCopy.front().second.first, this->Bonds.front().second.second - BondsCopy.front().second.second );
            
            bool Isomorphic = shift1 == shift2;
            unsigned int CurrentBond = 0;
            while( Isomorphic && CurrentBond < BondsCopy.size() )
            {
                int ShiftX = this->Bonds[ CurrentBond ].first.first - BondsCopy[ CurrentBond ].first.first;
                int ShiftY = this->Bonds[ CurrentBond ].first.second - BondsCopy[ CurrentBond ].first.second;

                Isomorphic = Isomorphic && (shift1 == std::make_pair( ShiftX, ShiftY ) );
                
                ShiftX = this->Bonds[ CurrentBond ].second.first - BondsCopy[ CurrentBond ].second.first;
                ShiftY = this->Bonds[ CurrentBond ].second.second - BondsCopy[ CurrentBond ].second.second;

                Isomorphic = Isomorphic && (shift2 == std::make_pair( ShiftX, ShiftY ) );
                
                CurrentBond++;
            }
            
            if( Isomorphic )
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return false;
    }
}

void BondGraph::AddBond( std::pair< int, int > FirstSite, std::pair< int, int > SecondSite )
{
    unsigned int InsertCounter = 0;
    while ( InsertCounter < this->Bonds.size() && this->Bonds[ InsertCounter ].first <= FirstSite && this->Bonds[ InsertCounter ].second <= SecondSite )
    {
        InsertCounter++;
    }
    this->Bonds.insert( this->Bonds.begin() + InsertCounter, std::make_pair( FirstSite, SecondSite ) );
}

void BondGraph::RemoveBond( std::pair< int, int > FirstSite, std::pair< int, int > SecondSite )
{
    unsigned int EraseCounter = 0;
    while( this->Bonds[ EraseCounter ].first <= FirstSite && this->Bonds[ EraseCounter ].second <= SecondSite && EraseCounter < this->Bonds.size() )
    {
        EraseCounter++;
    }
    this->Bonds.erase( this->Bonds.begin() + EraseCounter );
}

bool BondGraph::CheckForBond( std::pair< int, int > FirstSite, std::pair< int, int > SecondSite ) 
{
    std::pair< std::pair< int, int >, std::pair< int, int > > TempBond = std::make_pair( FirstSite, SecondSite ); 

    return std::binary_search( this->Bonds.begin(), this->Bonds.end(), TempBond );
}

int BondGraph::BondCount(std::pair< int, int > FirstSite, std::pair< int, int > SecondSite )
{
    int BondCounter = 0;
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++ )
    {

        if( (  this->Bonds[ CurrentBond ].first  == FirstSite  ||   
               this->Bonds[ CurrentBond ].first  == SecondSite ) ||
             ( this->Bonds[ CurrentBond ].second == SecondSite || 
               this->Bonds[ CurrentBond ].second == FirstSite ) )
        {
            BondCounter++;
        }
    }
    return BondCounter;
}

void BondGraph::PrintGraph()
{
    cout<<"ID number: "<<this->Identifier<<endl;
    cout<<"Order: "<<this->Order<<endl;
    cout<<"Lattice Constant: "<<this->LatticeConstant<<endl;
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++ )
    {
        cout<<"(";
        cout<<" (";
        cout<<this->Bonds[ CurrentBond ].first.first<<", "<<this->Bonds[ CurrentBond ].first.second<<") ";
        cout<<", (";
        cout<<this->Bonds[ CurrentBond ].second.first<<", "<<this->Bonds[ CurrentBond ].second.second<<")"<<" )";

    }
    cout<<endl;
}

void BondGraph::MakeCanonical()
{
    int GlobalGraphKey = 0;
    std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > CanonicalBonds;
    int MaxElement = this->DihedralSize();
    
    for( int currentFactor = 0; currentFactor < MaxElement; currentFactor++ )
    {
        int LocalGraphKey = 0;
        std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > BondsCopy = this->Bonds;
        Dihedral Transform( currentFactor, this->LatticeType );
        std::for_each( BondsCopy.begin(), BondsCopy.end(), Transform );
        for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++ )
        {
            std::pair< int, int > Temp = BondsCopy[ CurrentBond ].first;
            if( BondsCopy[ CurrentBond ].first > BondsCopy[ CurrentBond ].second )
            {
                BondsCopy[ CurrentBond ].first  = BondsCopy[ CurrentBond ].second;
                BondsCopy[ CurrentBond ].second = Temp;
            }
        }
        std::sort( BondsCopy.begin(), BondsCopy.end() );
        std::pair< int, int > shift = std::make_pair( -BondsCopy.front().first.first, -BondsCopy.front().first.second );
        for( unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++ )
        {
            BondsCopy[ CurrentBond ].first  = std::make_pair( BondsCopy[ CurrentBond ].first.first + shift.first, BondsCopy[ CurrentBond ].first.second + shift.second );
            BondsCopy[ CurrentBond ].second = std::make_pair( BondsCopy[ CurrentBond ].second.first + shift.first, BondsCopy[ CurrentBond ].second.second + shift.second );
            LocalGraphKey += BondsCopy[ CurrentBond ].first.first * this->Order + BondsCopy[ CurrentBond ].first.second;
            LocalGraphKey += BondsCopy[ CurrentBond ].second.first * this->Order + BondsCopy[ CurrentBond ].second.second;
        }
        if( LocalGraphKey > GlobalGraphKey )
        {
            GlobalGraphKey = LocalGraphKey;
            CanonicalBonds = BondsCopy;
        }
    }
    this->Bonds = CanonicalBonds;
}

void BondGraph::GenerateAdjacencyList()
{
    std::vector< std::pair< int, int > > SiteList;
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++ )
    {
        SiteList.push_back( this->Bonds[ CurrentBond ].first );
        SiteList.push_back( this->Bonds[ CurrentBond ].second );
    }
    std::sort( SiteList.begin(), SiteList.end() );
    std::vector< std::pair< int, int > >::iterator TrueEnd;
    TrueEnd = unique( SiteList.begin(), SiteList.end() );
    SiteList.resize( TrueEnd - SiteList.begin() );
    this->AdjacencyList.resize( this->Bonds.size() );
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++ )
    {
        std::vector< std::pair< int, int > >::iterator FirstSite;
        std::vector< std::pair< int, int > >::iterator SecondSite;
        FirstSite = std::find( SiteList.begin(), SiteList.end(), this->Bonds[ CurrentBond ].first );
        SecondSite = std::find( SiteList.begin(), SiteList.end(), this->Bonds[ CurrentBond ].second );
        this->AdjacencyList[ CurrentBond ] = std::make_pair( FirstSite - SiteList.begin(), SecondSite - SiteList.begin() ) ;
    }
    std::sort( this->AdjacencyList.begin(), this->AdjacencyList.end() );
}

void BondGraph::FindLatticeConstant()
{
    std::vector< std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > > BondLists;
    
    this->GenerateDistinctReps( BondLists );
    this->LatticeConstant = (int)BondLists.size();
}

int BondGraph::NumberSites()
{
    int max = 0;
    for( unsigned int CurrentBond = 0; CurrentBond < this->AdjacencyList.size(); CurrentBond++ )
    {
        max = ( this->AdjacencyList[ CurrentBond ].first > max )  ? this->AdjacencyList[ CurrentBond ].first  : max;
        max = ( this->AdjacencyList[ CurrentBond ].second > max ) ? this->AdjacencyList[ CurrentBond ].second : max;
    }
    max++;
    return max;
}

void BondGraph::GenerateDistinctReps( std::vector< std::vector< std::pair< std::pair< int, int >, std::pair< int, int> > > > & Representations )
{

    Representations.clear();
    Representations.push_back( this->Bonds );
    int MaxElement = this->DihedralSize();
    
    for( int CurrentElement = 1; CurrentElement < MaxElement; CurrentElement++ )
    {
        
        Dihedral Transform( CurrentElement, this->LatticeType );
        std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > ThisRep = this->Bonds;

        for_each( ThisRep.begin(), ThisRep.end(), Transform );
        for( unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++ )
        {
            std::pair<int,int> Temp = ThisRep[ CurrentBond ].first;
            if( ThisRep[ CurrentBond ].first > ThisRep[ CurrentBond ].second )
            {
                ThisRep[ CurrentBond ].first = ThisRep[ CurrentBond ].second;
                ThisRep[ CurrentBond ].second = Temp;
            }
        }
        std::sort( ThisRep.begin(), ThisRep.end() );
        bool GlobalShifted = false;
        for ( unsigned int CurrentRep = 0; CurrentRep < Representations.size(); CurrentRep++ )
        {
            const std::pair< int, int > shift1 = std::make_pair( Representations[ CurrentRep ].front().first.first - ThisRep.front().first.first, Representations[ CurrentRep ].front().first.second - ThisRep.front().first.second );
            const std::pair< int, int > shift2 = std::make_pair( Representations[ CurrentRep ].front().second.first - ThisRep.front().second.first, Representations[ CurrentRep ].front().second.second - ThisRep.front().second.second );
            bool Shifted = ( shift1 == shift2 );
            unsigned int CurrentBond = 1;
            while( Shifted && CurrentBond < Representations[ CurrentRep ].size() )
            {
                Shifted = Shifted && ( shift1.first  == ( Representations[ CurrentRep ][ CurrentBond ].first.first - ThisRep[ CurrentBond ].first.first ) ); 
                Shifted = Shifted && ( shift1.second == ( Representations[ CurrentRep ][ CurrentBond ].first.second - ThisRep[ CurrentBond ].first.second ) );
                Shifted = Shifted && ( shift2.first  == ( Representations[ CurrentRep ][ CurrentBond ].second.first - ThisRep[ CurrentBond ].second.first ) );
                Shifted = Shifted && ( shift2.second == ( Representations[ CurrentRep ][ CurrentBond ].second.second - ThisRep[ CurrentBond ].second.second ) );
                CurrentBond++;
            }

            GlobalShifted = GlobalShifted || Shifted;
        }
        if( !GlobalShifted )
        {
            const std::pair< int, int > shift = std::make_pair( -ThisRep.front().first.first, -ThisRep.front().first.second );
            for( unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++ )
            {
                ThisRep[ CurrentBond ].first.first += shift.first;
                ThisRep[ CurrentBond ].first.second += shift.second;
                ThisRep[ CurrentBond ].second.first += shift.first;
                ThisRep[ CurrentBond ].second.second += shift.second;
            }
            Representations.push_back( ThisRep );
        }
    }
}

void ConstructSiteBasedGraphs(std::vector< std::vector< SiteGraph > > & graphs, int FinalOrder )
{
    std::vector< SiteGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    while( CurrentOrder <= FinalOrder )
    {
        cout<<"Current Order: "<<CurrentOrder<<" Current Global ID: "<<GlobalIdentifier<<endl;   
        NewGraphs.clear();
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= graphs.back().size(); CurrentGraphGroup++ )
        {
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, NewGraphs, GlobalIdentifier) num_threads( MaxSize )
            {
                tid = omp_get_thread_num();
                gid = tid + CurrentGraphGroup * MaxSize;
                if( (unsigned int) gid < graphs.back().size() )
                {
                    std::vector< std::pair< int, int > > NewSites;
                    const SiteGraph OldGraph = graphs.back()[ gid ];
                    SiteGraph NewGraph;
                    
                    for( unsigned int CurrentSite = 0; CurrentSite < OldGraph.Sites.size(); CurrentSite++ )
                    {

                        switch( OldGraph.LatticeType )
                        {
                            case 0 : // Square
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first + 1, OldGraph.Sites[ CurrentSite ].second ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first, OldGraph.Sites[ CurrentSite ].second + 1 ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first - 1, OldGraph.Sites[ CurrentSite ].second ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first , OldGraph.Sites[ CurrentSite ].second - 1 ) );
                                break;

                            case 1 : // Triangle
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first + 1, OldGraph.Sites[ CurrentSite ].second ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first, OldGraph.Sites[ CurrentSite ].second + 1 ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first - 1, OldGraph.Sites[ CurrentSite ].second ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first , OldGraph.Sites[ CurrentSite ].second - 1 ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first - 1, OldGraph.Sites[ CurrentSite ].second + 1 ) );
                                NewSites.push_back( std::make_pair( OldGraph.Sites[ CurrentSite ].first + 1, OldGraph.Sites[ CurrentSite ].second - 1 ) );
                                break;
                            case 2 : //Kagome
                                break;
                            case 3 : //Honeycomb
                                break;
                        }
                        for( unsigned int CurrentNew = 0; CurrentNew < NewSites.size(); CurrentNew++ )
                        {
                            if( !std::binary_search( OldGraph.Sites.begin(), OldGraph.Sites.end(), NewSites[ CurrentNew ] ) )
                            {
                                NewGraph = OldGraph;
                                NewGraph.AddSite( NewSites[ CurrentNew ].first, NewSites[ CurrentNew ].second );
                                NewGraph.Order = OldGraph.Order + 1;
                                NewGraph.MakeCanonical();
                                NewGraph.GenerateAdjacencyList();
                                
                                bool Exists = false;
                        
                                #pragma omp critical
                                {
                                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                                    {
                                        Exists = Exists || ( NewGraph.Sites == NewGraphs[ CurrentIndex ].Sites );
                                        //Exists = Exists || (NewGraph.AdjacencyList == NewGraphs.at(CurrentIndex).AdjacencyList); 
                                    }
                                    if( !Exists )
                                    {
                                        NewGraph.Identifier = ++GlobalIdentifier;
                                        NewGraph.FindLatticeConstant();
                                        NewGraph.LowField = false;
                                        NewGraphs.push_back( NewGraph );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        graphs.insert( graphs.end(), NewGraphs );
        CurrentOrder++;
    }
}

void ConstructRectangularSiteGraphs( std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalWidth, unsigned int FinalHeight )
{
    graphs.resize( FinalWidth );
    int GlobalIdentifier = 0;
    
    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalWidth; CurrentGraphWidth++ )
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalHeight; CurrentGraphHeight++ )
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            std::vector< std::pair< int, int > > SiteList;
            SiteList.resize( CurrentOrder );
            std::vector< std::pair< int, int > > Subgraphs;
            for( unsigned int CurrentCheckWidth = 1; CurrentCheckWidth <= CurrentGraphWidth; CurrentCheckWidth++ )
            {
                for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight <= CurrentGraphHeight; CurrentCheckHeight++ )
                {
                    SiteList[ CurrentCheckHeight - 1 + ( CurrentCheckWidth - 1 ) * CurrentGraphHeight ] = std::make_pair( CurrentCheckWidth - 1, CurrentCheckHeight - 1 );

                    int SubgraphCount = ( CurrentGraphWidth + 1 - CurrentCheckWidth ) * ( CurrentGraphHeight + 1 - CurrentCheckHeight );
                    bool ThisGraph    = ( CurrentGraphHeight == CurrentCheckHeight ) && ( CurrentGraphWidth == CurrentCheckWidth );
                   
                    if( SubgraphCount > 0 && !ThisGraph )
                    {
                        Subgraphs.push_back( std::make_pair( SubgraphCount, graphs[ CurrentCheckWidth - 1 ][ CurrentCheckHeight - 1 ].Identifier ) );
                    }
                }
            }
            SiteGraph NewGraph( SiteList, GlobalIdentifier++, CurrentOrder, 1, Square, Subgraphs );
            NewGraph.GenerateAdjacencyList();
            NewGraph.LowField = false;
            graphs[ CurrentGraphWidth - 1 ].push_back( NewGraph );
        }
    }
}

void ConstructRectangularSiteGraphs( std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalOrder )
{
    graphs.resize( FinalOrder );
    int GlobalIdentifier = 0;
    

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalOrder; CurrentGraphWidth++ )
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalOrder && CurrentGraphHeight * CurrentGraphWidth <= FinalOrder; CurrentGraphHeight++ )
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            std::vector< std::pair< int, int > > SiteList;
            SiteList.resize( CurrentOrder );
            std::vector< std::pair< int, int > > Subgraphs;
            for( unsigned int CurrentCheckWidth = 1; CurrentCheckWidth <= CurrentGraphWidth; CurrentCheckWidth++ )
            {
                for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight <= CurrentGraphHeight; CurrentCheckHeight++ )
                {
                    SiteList[ CurrentCheckHeight - 1 + ( CurrentCheckWidth - 1 ) * CurrentGraphHeight ] = make_pair( CurrentCheckWidth - 1, CurrentCheckHeight - 1 );

                    int SubgraphCount = ( CurrentGraphWidth + 1 - CurrentCheckWidth ) * ( CurrentGraphHeight + 1 - CurrentCheckHeight );
                    bool ThisGraph    = ( CurrentGraphHeight == CurrentCheckHeight ) && ( CurrentGraphWidth == CurrentCheckWidth );
                   
                    if( SubgraphCount > 0 && !ThisGraph )
                    {
                        Subgraphs.push_back( std::make_pair( SubgraphCount, graphs[ CurrentCheckWidth - 1 ][ CurrentCheckHeight - 1 ].Identifier ) );
                    }
                }
            }
            SiteGraph NewGraph( SiteList, GlobalIdentifier++, CurrentOrder, 1, Square, Subgraphs );
            NewGraph.GenerateAdjacencyList();
            NewGraph.LowField = false;
            graphs[ CurrentGraphWidth - 1 ].push_back( NewGraph );
        }
    }
}

void ConstructBondBasedGraphs( std::vector< std::vector< BondGraph > > & graphs, int FinalOrder )
{
    std::vector< BondGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    while( CurrentOrder <= FinalOrder )
    {
        cout<<CurrentOrder<<endl;
        NewGraphs.clear();
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= graphs.back().size(); CurrentGraphGroup++ )
        {
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, NewGraphs, GlobalIdentifier) num_threads( MaxSize )
            {
                tid = omp_get_thread_num();
                gid = tid + CurrentGraphGroup * MaxSize;
                if( (unsigned int) gid < graphs.back().size() )
                {
                    BondGraph OldGraph = graphs.back()[ gid ];
                    std::vector< std::pair< std::pair< int, int >, std::pair< int, int > > > NewBonds;
                    std::pair< int, int > NewSite;
                    BondGraph NewGraph;
                    for( unsigned int CurrentBond = 0; CurrentBond < OldGraph.Bonds.size(); CurrentBond++ )
                    {
                        switch( OldGraph.LatticeType )
                        {
                            case 0 : //Square 
                                if( OldGraph.Bonds[ CurrentBond ].first.first != OldGraph.Bonds[ CurrentBond ].second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first, OldGraph.Bonds[ CurrentBond ].first.second + 1 ) ;
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].first, NewSite ) );  //Add a bond on the left, pointing up
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); //Add a bond on the right, pointing up
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the right, pointing right
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the left, pointing left
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first, OldGraph.Bonds[ CurrentBond ].first.second - 1 );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond to the left, pointing down
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond to the right, pointing down
                                }
                                else 
                                {
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the top, pointing up
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on top, pointing right
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first + 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].first, NewSite ) ); // Add a bond on the bottom, pointing right
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first - 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the top, pointing left
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond to the bottom, pointing left
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first, OldGraph.Bonds[ CurrentBond ].first.second - 1 );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond to the bottom, pointing down
                                }
                                break;
                            case 1 : //Triangle
                                //There are three possible bond orientations
                                if( OldGraph.Bonds[ CurrentBond ].second.first - OldGraph.Bonds[ CurrentBond ].first.first == 1 )
                                {
                                    if( OldGraph.Bonds[ CurrentBond ].second.second - OldGraph.Bonds[ CurrentBond ].first.second == 0 ) // Bond is horizontal
                                    {
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite) ); // Add a bond on the right, in the a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the right, in the a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first - 1, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the right, in the a_2 - a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second - 1);
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite) ); // Add a bond on the right, in the a_1 - a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the right, in the a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the left, in the -a_1 + a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the left, in the -a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first, OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the left, in the -a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the left, in the a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first + 1, OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the left, in the a_1 - a_2 direction
                                    }
                                    else // Bond is in the a_1 - a_2 direction
                                    {
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first - 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the bottom, in the -a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first , OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the bottom, in the -a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the bottom, in the a_1 - a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the bottom, in the a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second + 1);
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the bottom, in the a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the top, in the -a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first , OldGraph.Bonds[ CurrentBond ].first.second - 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the top, in the -a_2 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].first.second + 1 );
                                        NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the top, in the a_2 - a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first + 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].first, NewSite ) ); // Add a bond on the top, in the a_1 direction
                                        NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first, OldGraph.Bonds[ CurrentBond ].first.second + 1);
                                        NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].first, NewSite ) ); // Add a bond on the top, in the a_2 direction
                                    }
                                }
                                else //Bond is in the a_2 direction
                                {
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first - 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the top, in the -a_1 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first - 1, OldGraph.Bonds[ CurrentBond ].second.second + 1 );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].second ) ); // Add a bond on the top, in the a_2 - a_1 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second - 1 );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the top, in the a_1 - a_2 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first + 1, OldGraph.Bonds[ CurrentBond ].second.second );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the top, in the a_1 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].second.first, OldGraph.Bonds[ CurrentBond ].second.second + 1);
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].second, NewSite ) ); // Add a bond on the top, in the a_2 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the bottom, in the -a_1 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first , OldGraph.Bonds[ CurrentBond ].first.second - 1 );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the bottom, in the -a_2 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first - 1, OldGraph.Bonds[ CurrentBond ].first.second + 1 );
                                    NewBonds.push_back( std::make_pair( NewSite, OldGraph.Bonds[ CurrentBond ].first ) ); // Add a bond on the bottom, in the a_2 - a_1 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first + 1, OldGraph.Bonds[ CurrentBond ].first.second );
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].first, NewSite ) ); // Add a bond on the bottom, in the a_1 direction
                                    NewSite = std::make_pair( OldGraph.Bonds[ CurrentBond ].first.first + 1, OldGraph.Bonds[ CurrentBond ].first.second - 1);
                                    NewBonds.push_back( std::make_pair( OldGraph.Bonds[ CurrentBond ].first, NewSite ) ); // Add a bond on the bottom, in the a_1 - a_2 direction
                                }
                                break;
                            case 2 : //Kagome
                                break;
                            case 3 : //Honeycomb
                                break;
                        }
                        for( unsigned int CurrentCheck = 0; CurrentCheck < NewBonds.size(); CurrentCheck++ )
                        {
                    
                            if( !std::binary_search( OldGraph.Bonds.begin(), OldGraph.Bonds.end(), NewBonds[ CurrentCheck ] ) )
                            {
                                NewGraph = OldGraph;
                                NewGraph.AddBond( NewBonds[ CurrentCheck ].first, NewBonds[ CurrentCheck ].second );
                                NewGraph.Order = OldGraph.Order + 1;
                                NewGraph.MakeCanonical();
                                bool Exists = false;
                                #pragma omp critical
                                {
                                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                                    {
                                        Exists = Exists || (NewGraph == NewGraphs[ CurrentIndex ] ); 
                                    }
                                    if( !Exists )
                                    {
                                        NewGraph.Identifier = ++GlobalIdentifier;
                                        NewGraph.FindLatticeConstant();
                                        NewGraph.GenerateAdjacencyList();
                                        NewGraph.LowField = true;
                                        NewGraphs.push_back( NewGraph );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        graphs.insert( graphs.end(), NewGraphs );
        CurrentOrder++;
    }
}

void FindSubgraphs( std::vector< SiteGraph > & GraphList )
{
    int tid;
    int gid;
    int MaxSize = omp_get_num_procs();
    for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.size(); CurrentGraphGroup++ )
    {
        #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup) num_threads( MaxSize )
        {
            tid = omp_get_thread_num();
            gid = tid + CurrentGraphGroup * MaxSize;
            if ( (unsigned int) gid < GraphList.size() )
            {
                GraphList[ gid ].SubgraphList.push_back( std::make_pair( GraphList[ gid ].Order, 0 ) );
    
                unsigned int CurrentCheck = 1;
                while( CurrentCheck < (unsigned int) gid && GraphList[ CurrentCheck ].Order < GraphList[ gid ].Order )
                {
                    int EmbedCount = GraphList[ CurrentCheck ].Embeddings( GraphList[ gid ] ); 
                    //Now we have all distinct permutations of the graph!
                    if (EmbedCount > 0)
                    {
                        GraphList[ gid ].SubgraphList.push_back( std::make_pair( EmbedCount, GraphList[ CurrentCheck ].Identifier ) );
                    }
                    CurrentCheck++;
                }
            }
        }
    }
}

void FindSubgraphs( std::vector< std::vector< SiteGraph > > & GraphList )
{
    for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight < GraphList.size(); CurrentGraphHeight++ ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup * MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++ )
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + ( CurrentGraphGroup * MaxSize );
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {

                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair( GraphList[ CurrentGraphHeight ][ gid ].Order, 0) );
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for( unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {

                            int EmbedCount = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Embeddings( GraphList[ CurrentGraphHeight ][ gid ] );
                            if (EmbedCount > 0)
                            {
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(EmbedCount, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier ) );
                            }
                        }
                    }   
                }
            }
        }
    }
}

void FindSubgraphs( std::vector< std::vector< SiteGraph > > & GraphList, unsigned int Index )
{
    unsigned int CurrentGraphHeight = Index - 1;
    if ( CurrentGraphHeight < GraphList.size() ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++ )
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + ( CurrentGraphGroup * MaxSize );
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {

                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.clear();
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(GraphList[ CurrentGraphHeight ][ gid ].Order, 0) );
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for( unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {

                            int EmbedCount = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Embeddings( GraphList[ CurrentGraphHeight ][ gid ] );
                            if (EmbedCount > 0)
                            {
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(EmbedCount, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier ) );
                            }
                        }
                    }   
                }
            }
        }
    }
}

void FindSubgraphs( vector< vector< BondGraph > > & GraphList )
{
    for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight < GraphList.size(); CurrentGraphHeight++ ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++ )
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + ( CurrentGraphGroup * MaxSize );
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair( GraphList[ CurrentGraphHeight ][ gid ].NumberSites(), 0 ) );
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for( unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {
                            
                            int EmbedCount = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Embeddings( GraphList[ CurrentGraphHeight ][ gid ] );

                            if( EmbedCount > 0 )
                            {
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair( EmbedCount, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier ) );
                            }
                        }
                    }
                }
            }
        }
    }
}

void FindSubgraphs( vector< vector< BondGraph > > & GraphList, unsigned int Index )
{
    unsigned int CurrentGraphHeight = Index - 1;
    if( CurrentGraphHeight < GraphList.size() ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++ )
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + ( CurrentGraphGroup * MaxSize );
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.clear();
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back(std::make_pair(GraphList[ CurrentGraphHeight ][ gid ].NumberSites(), 0 ) );
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for( unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {
                            int EmbedCount = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Embeddings( GraphList[ CurrentGraphHeight ][ gid ] );
                            if( EmbedCount > 0 )
                            {
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair( EmbedCount, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier ) );
                            }
                        }
                    }
                }
            }
        }
    }
}

void WriteGraphsToFile( std::vector< SiteGraph > & GraphList, string File )
{
    ofstream Output( File.c_str() );
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++ )
    {
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<GraphList.at(CurrentGraph).LowField<<endl;

        for( unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentGraph).Sites.size(); CurrentSite++ )
        {
            Output<<GraphList.at(CurrentGraph).Sites.at(CurrentSite).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).Sites.at(CurrentSite).second;
            Output<<" ";
        }
        Output<<endl;
        for( unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentGraph).AdjacencyList.size(); CurrentConnection++ )
        {
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).second;
            Output<<" ";
        }
        Output<<endl;
        for( unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentGraph).SubgraphList.size(); CurrentSubgraph++ )
        {
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
        }
        Output<<endl;
    }
}

void WriteGraphsToFile( std::vector< std::vector<SiteGraph> > & GraphList, string File )
{
    ofstream Output( File.c_str() );
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++ )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++ )
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

            for( unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentWidth).at(CurrentHeight).Sites.size(); CurrentSite++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
            }
            Output<<endl;
        }
    }
}

void WriteGraphsToFile( std::vector< std::vector< SiteGraph > > & GraphList, string File, unsigned int Index )
{
    ofstream Output;
    Output.open( File.c_str(), std::ios::app );
    const unsigned int CurrentWidth = Index - 1;
    if( CurrentWidth < GraphList.size() )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++ )
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;
            for( unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentWidth).at(CurrentHeight).Sites.size(); CurrentSite++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
            }
            Output<<endl;
        }
    }
    Output.close();
}

void WriteGraphsToFile( std::vector< BondGraph > & GraphList, string File )
{
    ofstream Output( File.c_str() );
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++ )
    {
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<GraphList.at(CurrentGraph).LowField<<endl;

        for( unsigned int CurrentBond = 0; CurrentBond < GraphList.at(CurrentGraph).Bonds.size(); CurrentBond++ )
        {
            Output<<GraphList.at(CurrentGraph).Bonds.at(CurrentBond).first.first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).Bonds.at(CurrentBond).first.second;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).Bonds.at(CurrentBond).second.first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).Bonds.at(CurrentBond).second.second;
            Output<<" ";
        }
        Output<<endl;
        for( unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentGraph).AdjacencyList.size(); CurrentConnection++ )
        {
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).second;
            Output<<" ";
        }
        Output<<endl;
        for( unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentGraph).SubgraphList.size(); CurrentSubgraph++ )
        {
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
        }
        Output<<endl;
    }
    Output.close();
}

void WriteGraphsToFile( std::vector< std::vector< BondGraph > > & GraphList, string File )
{
    ofstream Output( File.c_str() );
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++ )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++ )
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

            for( unsigned int CurrentBond = 0; CurrentBond < GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.size(); CurrentBond++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).first.first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).first.second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).second.first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).second.second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
            } 
            Output<<endl;
        }
    }
    Output.close();
}

void WriteGraphsToFile( std::vector< std::vector< BondGraph> > & GraphList, string File, unsigned int Index )
{
    ofstream Output;
    Output.open( File.c_str(), std::ios::app );
    const unsigned int CurrentWidth = Index - 1;
    if( CurrentWidth < GraphList.size() )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++ )
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

            for( unsigned int CurrentBond = 0; CurrentBond < GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.size(); CurrentBond++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).first.first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).first.second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).second.first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.at(CurrentBond).second.second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            for( unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++ )
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
            } 
            Output<<endl;
        }
    }
    Output.close();
}

void ReadGraphsFromFile( vector< Graph > & GraphList, const string & File )
{
  ifstream input( File.c_str() );
  vector< string > rawLines;
  Graph tempGraph;
  
  while ( !input.eof() )
    {
        rawLines.resize(rawLines.size() + 1);
        getline(input, rawLines.back()) ; 
    }

    input.close();

    //cout<<rawLines.size()<<endl;

    stringstream ss (stringstream::in | stringstream::out);

    for  (unsigned int currentLine = 0; currentLine < rawLines.size()-1; currentLine+=4)
    {

        string currentNumber;
    
        ss << rawLines.at(currentLine);
          
        ss >> tempGraph.Identifier;
        ss >> tempGraph.Order;
    //    ss >> tempGraph.NumberBonds;
        ss >> tempGraph.LatticeConstant;
        ss >> tempGraph.LowField;
           
        //cout << "Identifier = " <<tempGraph.Identifier << endl;
        //cout << "NumberSites = " << tempGraph.NumberSites << endl;
        //cout << "LatticeConstant = " <<tempGraph.LatticeConstant << endl;
        //cout << "LowField = " << tempGraph.LowField << endl;

        ss.str("");
        ss.clear();

        //read in bonds
        ss << rawLines.at(currentLine+2);   
        int subSize(0);
        string teststring;
        while(!ss.eof()){
            ss >> teststring;
            subSize++;
        }
        int NumberBonds = subSize/2;
        ss.str("");
        ss.clear();
        teststring="";
        
        ss << rawLines.at(currentLine+2);

        tempGraph.AdjacencyList.resize(NumberBonds);
        for(int b=0; b<NumberBonds;b++){
          ss >> tempGraph.AdjacencyList[b].first;
          ss >> tempGraph.AdjacencyList[b].second;
          //cout << tempGraph.AdjacencyList[b].first << "," <<tempGraph.AdjacencyList[b].second << endl;
        }
        ss.str("");
        ss.clear();



        //read in subclusters
        ss << rawLines.at(currentLine+3);   

        subSize=0;
        while(!ss.eof()){
            ss >> teststring;
            subSize++;
        }
        subSize/=2; // cout << subSize << endl;

        ss.str("");
        ss.clear();
        ss << rawLines.at(currentLine+3);
        tempGraph.SubgraphList.resize(subSize);
        for(int b=0; b<subSize;b++){
            ss >> tempGraph.SubgraphList[b].first;
            ss >> tempGraph.SubgraphList[b].second;
            //  cout << tempGraph.SubgraphList[b].first << "," <<tempGraph.SubgraphList[b].second << endl;
        }

        ss.str("");
        ss.clear();

        GraphList.push_back(tempGraph);

    }   
}
