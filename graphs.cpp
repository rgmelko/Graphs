#include "graphs.h"

int main()
{
    vector< vector< SiteGraph > > graphs;
    graphs.resize(1);
    graphs.front().resize(1);
    vector<pair<int,int> > Origin;
    Origin.resize(1);
    Origin.at(0) = make_pair(0,0);
    vector< int > NoSubgraphs;
    SiteGraph BareSite(Origin, 1, 1, 0, NoSubgraphs);
    graphs.front().at(0) = BareSite;
    ConstructSiteBasedGraphs(graphs, 5);
    return 0;
}

Graph::Graph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    SubgraphList.clear();
}

Graph::Graph(int IdentNumber, int order, int LattConst, vector< int > & subgraphs )
{
    Identifier = IdentNumber;
    Order = order;
    LatticeConstant = LattConst;
    SubgraphList = subgraphs;
}

Graph& Graph::operator=( const Graph & other)
{
    this->Order = other.Order;
    this->LatticeConstant = other.LatticeConstant;
    this->SubgraphList = other.SubgraphList;
    return *this;
}

bool Graph::operator==( const Graph & other)
{
    return (( this->Order == other.Order) &&
           ( this->LatticeConstant == other.LatticeConstant) );
}

SiteGraph::SiteGraph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    Sites.clear();
    SubgraphList.clear();
}

SiteGraph::SiteGraph(vector<pair <int, int> > & siteList, int IdentNumber, int order, int LattConst, vector< int > & subgraphs )
{
    Identifier = IdentNumber;
    Order = order;
    LatticeConstant = LattConst;
    SubgraphList = subgraphs;
    Sites = siteList;
}

SiteGraph& SiteGraph::operator=( const SiteGraph & other)
{
    this->Order = other.Order;
    this->LatticeConstant = other.LatticeConstant;
    this->SubgraphList = other.SubgraphList;
    this->Sites = other.Sites;
    return *this;
}

void SiteGraph::AddSite(int xIndex, int yIndex)
{
    unsigned int InsertCounter = 0;
    while (  InsertCounter < this->Sites.size() && this->Sites.at(InsertCounter).first <= xIndex && this->Sites.at(InsertCounter).second <= yIndex )
    {
        InsertCounter++;
    }
    this->Sites.insert( this->Sites.begin() + InsertCounter, make_pair(xIndex, yIndex));
}

void SiteGraph::RemoveSite(int xIndex, int yIndex)
{
    unsigned int EraseCounter = 0;
    while ( this->Sites.at(EraseCounter).first <= xIndex && this->Sites.at(EraseCounter).second <= yIndex && EraseCounter< this->Sites.size())
    {
        EraseCounter++;
    }
    this->Sites.erase( this->Sites.begin() + EraseCounter);
}

bool SiteGraph::CheckForSite(int xIndex, int yIndex)
{
    pair<int, int> TempSite = make_pair(xIndex, yIndex); 

    return binary_search(this->Sites.begin(), this->Sites.end(), TempSite);
}

Dihedral::Dihedral()
{
    element = 0;
}

Dihedral::Dihedral(int factor)
{
    element = factor;
}

void Dihedral::operator()(pair<int, int> Coordinates)
{
    int TempFirst;
    int TempSecond;
    switch( this->element )
    {
        case 0 :
            break;
        case 1 :
            TempFirst = Coordinates.first;
            Coordinates.first = -Coordinates.second;
            Coordinates.second = TempFirst;
            break;
        case 2 : 
            Coordinates.first = -Coordinates.first;
            Coordinates.second = -Coordinates.second;
            break;
        case 3 :
            TempSecond = Coordinates.second;
            Coordinates.second = -Coordinates.first;
            Coordinates.first = TempSecond;
            break;
        case 4 : 
            Coordinates.first = -Coordinates.first;
            break;
        case 5 :
            Coordinates.second = -Coordinates.second;
            break;
        case 6 :
            TempFirst = -Coordinates.first;
            TempSecond = -Coordinates.second;
            Coordinates.first = TempSecond;
            Coordinates.second = TempFirst;
            break;
        case 7 : 
            TempFirst = Coordinates.first;
            TempSecond = Coordinates.second;
            Coordinates.first = TempSecond;
            Coordinates.second = TempFirst;
            break;
    }
}

void Dihedral::operator()(pair< pair<int, int>, pair<int,int> > Coordinates)
{
    int TempFirst;
    int TempSecond;
    
    switch( this->element )
    {
        case 0 :
            break;
        case 1 :
            TempFirst = Coordinates.first.first;
            Coordinates.first.first = -Coordinates.first.second;
            Coordinates.first.second = TempFirst;
            TempFirst = Coordinates.second.first;
            Coordinates.second.first = -Coordinates.second.second;
            Coordinates.second.second = TempFirst;
            break;
        case 2 : 
            Coordinates.first.first = -Coordinates.first.first;
            Coordinates.first.second = -Coordinates.first.second;
            Coordinates.second.first = -Coordinates.second.first;
            Coordinates.second.second = -Coordinates.second.second;
            break;
        case 3 :
            TempSecond = Coordinates.first.second;
            Coordinates.first.second = -Coordinates.first.first;
            Coordinates.first.first = TempSecond;
            TempSecond = Coordinates.second.second;
            Coordinates.second.second = -Coordinates.second.first;
            Coordinates.second.first = TempSecond;
            break;
        case 4 : 
            Coordinates.first.first = -Coordinates.first.first;
            Coordinates.second.first = -Coordinates.second.first;
            break;
        case 5 :
            Coordinates.first.second = -Coordinates.first.second;
            Coordinates.second.second = -Coordinates.second.second;
            break;
        case 6 :
            TempFirst = -Coordinates.first.first;
            TempSecond = -Coordinates.first.second;
            Coordinates.first.first = TempSecond;
            Coordinates.first.second = TempFirst;
            TempFirst = -Coordinates.second.first;
            TempSecond = -Coordinates.second.second;
            Coordinates.second.first = TempSecond;
            Coordinates.second.second = TempFirst;
            break;
        case 7 : 
            TempFirst = Coordinates.first.first;
            TempSecond = Coordinates.first.second;
            Coordinates.first.first = TempSecond;
            Coordinates.first.second = TempFirst;
            TempFirst = Coordinates.second.first;
            TempSecond = Coordinates.second.second;
            Coordinates.second.first = TempSecond;
            Coordinates.second.second = TempFirst;
            break;
    }
}

bool SiteGraph::operator==(const SiteGraph & other)
{
    if( ( other.LatticeConstant == this->LatticeConstant) &&
        ( other.Order == this->Order) )
    {
        for( int currentFactor = 0; currentFactor < 8; currentFactor++)
        {
            vector<pair<int,int> > SitesCopy = other.Sites;
            Dihedral Transform(currentFactor);
            for_each(SitesCopy.begin(), SitesCopy.end(), Transform);
            sort(SitesCopy.begin(), SitesCopy.end());
            pair<int,int> shift = make_pair(-SitesCopy.front().first, -SitesCopy.front().second);
            for(unsigned int CurrentSite = 0; CurrentSite < SitesCopy.size(); CurrentSite++)
            {
                SitesCopy.at(CurrentSite) = make_pair(SitesCopy.at(CurrentSite).first + shift.first, SitesCopy.at(CurrentSite).second + shift.second);
            }
            if (SitesCopy == this->Sites)
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

int SiteGraph::SiteDegree(int xIndex, int yIndex)
{
    int DegreeCounter = 0;
    for( unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++)
    {
        if( ( (this->Sites.at(CurrentSite).first == xIndex - 1 || 
               this->Sites.at(CurrentSite).first == xIndex + 1) && 
               this->Sites.at(CurrentSite).second == yIndex) ||
            ( (this->Sites.at(CurrentSite).second == yIndex - 1 || 
               this->Sites.at(CurrentSite).second == yIndex + 1) &&
               this->Sites.at(CurrentSite).first == xIndex) )
         {
            DegreeCounter++;
         }
    }
    return DegreeCounter;
}

void SiteGraph::MakeCanonical()
{
    unsigned int GlobalGraphKey = 0;
    vector< pair<int,int> > CanonicalSites;

    for( int currentFactor = 0; currentFactor < 8; currentFactor++)
    {
        unsigned int LocalGraphKey = 0;
        vector< pair<int,int> > SitesCopy = this->Sites;
        for_each(SitesCopy.begin(), SitesCopy.end(), Transform);
        sort(SitesCopy.begin(), SitesCopy.end());
        pair<int,int> shift = make_pair(-SitesCopy.front().first, -SitesCopy.front().second);
        for(unsigned int CurrentSite = 0; CurrentSite < SitesCopy.size(); CurrentSite++)
        {
            SitesCopy.at(CurrentSite) = make_pair(SitesCopy.at(CurrentSite).first + shift.first, SitesCopy.at(CurrentSite).second + shift.second);
            LocalGraphKey += SitesCopy.at(CurrentSite).first*this->Order - SitesCopy.at(CurrentSite).second;
        }
        if( LocalGraphKey > GlobalGraphKey )
        {
            GlobalGraphKey = LocalGraphKey;
            CanonicalSites = SitesCopy;
        }
    }
    this->Sites = CanonicalSites;
}

BondGraph::BondGraph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    Bonds.clear();
    SubgraphList.clear();
}

BondGraph::BondGraph(vector< pair< pair <int, int>, pair<int, int> > > & bondList, int IdentNumber, int order, int LattConst, vector< int > & subgraphs )
{
    Identifier = IdentNumber;
    Order = order;
    LatticeConstant = LattConst;
    SubgraphList = subgraphs;
    Bonds = bondList;
}

BondGraph& BondGraph::operator=( const BondGraph & other)
{
    this->Order = other.Order;
    this->LatticeConstant = other.LatticeConstant;
    this->SubgraphList = other.SubgraphList;
    this->Bonds = other.Bonds;
    return *this;
}

bool BondGraph::operator==( const BondGraph & other)
{
    if( ( other.LatticeConstant == this->LatticeConstant) &&
        ( other.Order == this->Order) )
    {
        for( int currentFactor = 0; currentFactor < 8; currentFactor++)
        {
            vector<pair< pair<int,int>, pair<int,int> > > BondsCopy = other.Bonds;
            Dihedral Transform(currentFactor);
            for_each(BondsCopy.begin(), BondsCopy.end(), Transform);
            for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
            {
                pair<int,int> Temp = BondsCopy.at(CurrentBond).first;
                if( BondsCopy.at(CurrentBond).first <= BondsCopy.at(CurrentBond).second)
                {
                    BondsCopy.at(CurrentBond).first = BondsCopy.at(CurrentBond).second;
                    BondsCopy.at(CurrentBond).second = Temp;
                }
            }
            sort(BondsCopy.begin(), BondsCopy.end());
            pair<int, int> shift = BondsCopy.front().first;
            for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
            {
                BondsCopy.at(CurrentBond).first = make_pair( BondsCopy.at(CurrentBond).first.first +shift.first, BondsCopy.at(CurrentBond).first.second + shift.second);
                BondsCopy.at(CurrentBond).second = make_pair(BondsCopy.at(CurrentBond).second.first + shift.first, BondsCopy.at(CurrentBond).second.second + shift.second);
            }
            if (BondsCopy == this->Bonds)
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

void BondGraph::AddBond(pair<int, int> firstSite, pair<int, int> secondSite )
{
    unsigned int InsertCounter = 0;
    while ( this->Bonds.at(InsertCounter).first <= firstSite && this->Bonds.at(InsertCounter).second <= secondSite && InsertCounter< this->Bonds.size())
    {
        InsertCounter++;
    }
    this->Bonds.insert( this->Bonds.begin() + InsertCounter, make_pair(firstSite, secondSite));
}

void BondGraph::RemoveBond( pair<int, int> firstSite, pair<int, int> secondSite)
{
    unsigned int EraseCounter = 0;
    while ( this->Bonds.at(EraseCounter).first <= firstSite && this->Bonds.at(EraseCounter).second <= secondSite && EraseCounter< this->Bonds.size())
    {
        EraseCounter++;
    }
    this->Bonds.erase( this->Bonds.begin() + EraseCounter);
}

bool BondGraph::CheckForBond(pair<int,int> firstSite, pair<int,int> secondSite)
{
    pair<pair<int, int>, pair<int, int> > TempBond = make_pair(firstSite, secondSite); 

    return binary_search(this->Bonds.begin(), this->Bonds.end(), TempBond);
}

int BondGraph::BondCount(pair<int,int> FirstSite, pair<int,int> SecondSite)
{
    int BondCounter = 0;
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++)
    {

        if( ( this->Bonds.at(CurrentBond).first == FirstSite  ||   
              this->Bonds.at(CurrentBond).first == SecondSite) ||
             (this->Bonds.at(CurrentBond).second == SecondSite || 
              this->Bonds.at(CurrentBond).second == FirstSite) )
         {
            BondCounter++;
         }
    }
    return BondCounter;
}

void ConstructSiteBasedGraphs(vector< vector< SiteGraph > > graphs, int FinalOrder)
{
    vector< SiteGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    
    while (CurrentOrder <= FinalOrder)
    {
        for( unsigned int CurrentGraph = 0; CurrentGraph < graphs.back().size(); CurrentGraph++)
        {
            SiteGraph OldGraph = graphs.back().at(CurrentGraph);
            for( unsigned int CurrentSite = 0; CurrentSite < OldGraph.Sites.size(); CurrentSite++)
            {
                pair<int,int> EastSite = make_pair(OldGraph.Sites.at(CurrentSite).first + 1, OldGraph.Sites.at(CurrentSite).second);
                pair<int,int> NorthSite = make_pair(OldGraph.Sites.at(CurrentSite).first , OldGraph.Sites.at(CurrentSite).second + 1);
            
                if( !binary_search( OldGraph.Sites.begin(), OldGraph.Sites.end(), EastSite))
                {
                    SiteGraph NewGraph;
                    NewGraph = OldGraph;
                    NewGraph.AddSite( EastSite.first, EastSite.second);
                    NewGraph.Order = OldGraph.Order + 1;
                    NewGraph.SubgraphList.push_back( OldGraph.Identifier );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists |= (NewGraph.Sites == NewGraphs.at(CurrentIndex).Sites ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraphs.push_back( NewGraph );
                    }
                
                }
                if( !binary_search( OldGraph.Sites.begin(), OldGraph.Sites.end(), NorthSite))
                {
                    SiteGraph NewGraph;
                    NewGraph = OldGraph;
                    NewGraph.AddSite(NorthSite.first, NorthSite.second);
                    NewGraph.Order = OldGraph.Order + 1;
                    NewGraph.SubgraphList.push_back( OldGraph.Identifier );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists |= (NewGraph.Sites == NewGraphs.at(CurrentIndex).Sites ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraphs.push_back(NewGraph);
                    }
                }
            }
        }
        graphs.push_back(NewGraphs);
        CurrentOrder++;
    }
}
