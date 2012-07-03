#include "graphs.h"

int main()
{
    //vector< vector< SiteGraph > > rectangles;
    //ConstructRectangularSiteGraphs(rectangles, 6);
    vector< vector< SiteGraph > > testsites;
    testsites.resize(1);
    vector< pair<int,int> > SiteList;
    SiteList.resize(1);
    SiteList[0].first = 0;
    SiteList[0].second = 0;
    vector<pair<int,int> > Empty;
    SiteGraph Start(SiteList, 0, 1, 1, Empty); 
    testsites[0].resize(1);
    testsites[0][0] = Start;
    ConstructSiteBasedGraphs(testsites, 4);
    FindSubgraphs(testsites);
    WriteGraphsToFile(testsites, "allsitebased.dat");
    /*vector< vector< BondGraph > > testbonds;
    testbonds.resize(1);
    vector<pair< pair<int,int>, pair<int,int> > > BondList;
    BondList.resize(1);
    BondList[0].first.first = 0;
    BondList[0].first.second = 0;
    BondList[0].second.first = 1;
    BondList[0].second.second = 0;
    vector<pair<int,int> > Empty;
    BondGraph Start(BondList, 0, 1, 1, Empty); 
    testbonds[0].resize(1);
    testbonds[0][0] = Start;
    ConstructBondBasedGraphs(testbonds, 7);*/
    /*for( unsigned int i = 0; i < testsites.size(); i++)
    {
        for( unsigned int j = 0; j < testsites.at(i).size(); j++)
        {
            testsites.at(i).at(j).PrintGraph();
        }
    }*/
    //WriteGraphsToFile(rectangles, "rectanglegraphs.dat");
    return 0;

}

Graph::Graph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    SubgraphList.clear();
}

Graph::Graph(int IdentNumber, int order, int LattConst, vector< pair<int, int> > & subgraphs )
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

SiteGraph::SiteGraph(vector<pair <int, int> > & siteList, int IdentNumber, int order, int LattConst, vector< pair<int, int> > & subgraphs )
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

void SiteGraph::PrintGraph()
{
    cout<<"ID number: "<<this->Identifier<<endl;
    cout<<"Order: "<<this->Order<<endl;
    cout<<"Lattice Constant: "<<this->LatticeConstant<<endl;
    for( unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++)
    {
        cout<<"("<<Sites.at(CurrentSite).first<<", "<<Sites.at(CurrentSite).second<<") ";
    }
    cout<<endl;
    for ( unsigned int CurrentSubgraph = 0; CurrentSubgraph < this->SubgraphList.size(); CurrentSubgraph++)
    {
        cout<<"("<<SubgraphList.at(CurrentSubgraph).first<<", "<<SubgraphList.at(CurrentSubgraph).second<<") ";
    }
    cout<<endl;
}

void SiteGraph::FindLatticeConstant()
{
    vector< vector< pair<int,int> > > SiteLists;
    //sort(this->Sites.begin(), this->Sites.end());
    SiteLists.push_back(this->Sites);
    int Counter = 1;
    for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++ )
    {
        vector< pair<int,int> > TempSites = this->Sites;
        Dihedral Transform(CurrentElement);
        for_each(TempSites.begin(), TempSites.end(), Transform);
        sort(TempSites.begin(), TempSites.end());
        bool GlobalShifted = false;
        for ( unsigned int CurrentList = 0; CurrentList < SiteLists.size(); CurrentList++ )
        {
            bool Shifted = true;
            const pair< int, int> shift = make_pair(SiteLists.at(CurrentList).front().first - TempSites.front().first, SiteLists.at(CurrentList).front().second - TempSites.front().second);
            
            unsigned int CurrentSite = 1;
            while( Shifted && CurrentSite < SiteLists.at(CurrentList).size())
            {
                Shifted = Shifted && ((shift.first == (SiteLists.at(CurrentList).at(CurrentSite).first - TempSites.at(CurrentSite).first)) && (shift.second == (SiteLists.at(CurrentList).at(CurrentSite).second - TempSites.at(CurrentSite).second)));
                CurrentSite++;
            }
            
            GlobalShifted = GlobalShifted || Shifted;
        }
        if (!GlobalShifted)
        {
            SiteLists.push_back(TempSites);
            Counter++;
        }
    }

    this->LatticeConstant = Counter;
}

Dihedral::Dihedral()
{
    element = 0;
}

Dihedral::Dihedral(int factor)
{
    element = factor;
}

void Dihedral::operator()(pair<int, int> & Coordinates)
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

void Dihedral::operator()(pair< pair<int, int>, pair<int,int> > & Coordinates)
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
            bool Isomorphic = true; 
            const pair<int,int> shift = make_pair(this->Sites.front().first - SitesCopy.front().first, this->Sites.front().second - SitesCopy.front().second);
            unsigned int CurrentSite = 0;
            while(Isomorphic && CurrentSite < SitesCopy.size())
            {
                Isomorphic = (Isomorphic) && (shift.first == (this->Sites.at(CurrentSite).first - SitesCopy.at(CurrentSite).first)) && (shift.second == (this->Sites.at(CurrentSite).second - SitesCopy.at(CurrentSite).second ));
                CurrentSite++;
            }
            if (Isomorphic)
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
    int GlobalGraphKey = 0;
    vector< pair<int,int> > CanonicalSites;

    for( int currentFactor = 0; currentFactor < 8; currentFactor++)
    {
        int LocalGraphKey = 0;
        vector< pair<int,int> > SitesCopy = this->Sites;
        Dihedral Transform(currentFactor);
        for_each(SitesCopy.begin(), SitesCopy.end(), Transform);
        sort(SitesCopy.begin(), SitesCopy.end());
        pair<int,int> shift = make_pair(-SitesCopy.front().first, -SitesCopy.front().second);
        for(unsigned int CurrentSite = 0; CurrentSite < SitesCopy.size(); CurrentSite++)
        {
            SitesCopy.at(CurrentSite) = make_pair(SitesCopy.at(CurrentSite).first + shift.first, SitesCopy.at(CurrentSite).second + shift.second);
            LocalGraphKey += SitesCopy.at(CurrentSite).first*this->Order + SitesCopy.at(CurrentSite).second;
        }
        if( LocalGraphKey > GlobalGraphKey )
        {
            GlobalGraphKey = LocalGraphKey;
            CanonicalSites = SitesCopy;
        }
    }
    this->Sites = CanonicalSites;
}

void SiteGraph::GenerateAdjacencyList()
{
    for (unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++)
    {
        for (unsigned int CurrentIndex = CurrentSite + 1; CurrentIndex < this->Sites.size(); CurrentIndex++)
        {
            if( ( Sites.at(CurrentIndex).first == Sites.at(CurrentSite).first && 
                  Sites.at(CurrentIndex).second == Sites.at(CurrentSite).second + 1)  ||
                ( Sites.at(CurrentIndex).second == Sites.at(CurrentSite).second && 
                  Sites.at(CurrentIndex).first == Sites.at(CurrentSite).first + 1) ) 
            {
                AdjacencyList.push_back(make_pair(CurrentSite, CurrentIndex) );
            }
        }
    }

}

BondGraph::BondGraph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    Bonds.clear();
    SubgraphList.clear();
}

BondGraph::BondGraph(vector< pair< pair <int, int>, pair<int, int> > > & bondList, int IdentNumber, int order, int LattConst, vector< pair< int, int > > & subgraphs )
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
                if( BondsCopy.at(CurrentBond).first > BondsCopy.at(CurrentBond).second)
                {
                    BondsCopy.at(CurrentBond).first = BondsCopy.at(CurrentBond).second;
                    BondsCopy.at(CurrentBond).second = Temp;
                }
            }
            sort(BondsCopy.begin(), BondsCopy.end());
            pair<int, int> shift1 = make_pair(this->Bonds.front().first.first - BondsCopy.front().first.first, this->Bonds.front().first.second - BondsCopy.front().first.second);
            pair<int, int> shift2 = make_pair(this->Bonds.front().second.first - BondsCopy.front().second.first, this->Bonds.front().second.second - BondsCopy.front().second.second);
            //cout<<"Shifts equal: "<<(shift1 == shift2)<<endl;
            bool Isomorphic = shift1 == shift2;
            unsigned int CurrentBond = 0;
            while( Isomorphic && CurrentBond < BondsCopy.size())
            {
                Isomorphic = Isomorphic && (shift1 == make_pair(this->Bonds.at(CurrentBond).first.first - BondsCopy.at(CurrentBond).first.first, this->Bonds.at(CurrentBond).first.second - BondsCopy.at(CurrentBond).first.second));
                Isomorphic = Isomorphic && (shift2 == make_pair(this->Bonds.at(CurrentBond).second.first - BondsCopy.at(CurrentBond).second.first, this->Bonds.at(CurrentBond).second.second - BondsCopy.at(CurrentBond).second.second));
                CurrentBond++;
            }
            if (Isomorphic)
            {
                /*for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
                {
                    pair<int,int> Temp = BondsCopy.at(CurrentBond).first;
                    if( BondsCopy.at(CurrentBond).first <= BondsCopy.at(CurrentBond).second)
                    {
                        BondsCopy.at(CurrentBond).first = BondsCopy.at(CurrentBond).second;
                        BondsCopy.at(CurrentBond).second = Temp;
                    }
                }*/
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
    while ( InsertCounter < this->Bonds.size() && this->Bonds.at(InsertCounter).first <= firstSite && this->Bonds.at(InsertCounter).second <= secondSite )
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

void BondGraph::PrintGraph()
{
    cout<<"ID number: "<<this->Identifier<<endl;
    cout<<"Order: "<<this->Order<<endl;
    cout<<"Lattice Constant: "<<this->LatticeConstant<<endl;
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++)
    {
        cout<<"(";
        cout<<" (";
        cout<<this->Bonds.at(CurrentBond).first.first<<", "<<this->Bonds.at(CurrentBond).first.second<<") ";
        cout<<", (";
        cout<<this->Bonds.at(CurrentBond).second.first<<", "<<this->Bonds.at(CurrentBond).second.second<<")"<<" )";

    }
    cout<<endl;
}
void BondGraph::MakeCanonical()
{
    int GlobalGraphKey = 0;
    vector<pair< pair<int,int>, pair<int,int> > > CanonicalBonds;

    for( int currentFactor = 0; currentFactor < 8; currentFactor++)
    {
        int LocalGraphKey = 0;
        vector< pair<pair<int,int>, pair<int,int> > > BondsCopy = this->Bonds;
        Dihedral Transform(currentFactor);
        for_each(BondsCopy.begin(), BondsCopy.end(), Transform);
        for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
        {
            pair<int,int> Temp = BondsCopy.at(CurrentBond).first;
            if( BondsCopy.at(CurrentBond).first > BondsCopy.at(CurrentBond).second)
            {
                BondsCopy.at(CurrentBond).first = BondsCopy.at(CurrentBond).second;
                BondsCopy.at(CurrentBond).second = Temp;
            }
        }
        sort(BondsCopy.begin(), BondsCopy.end());
        pair<int,int> shift = make_pair(-BondsCopy.front().first.first, -BondsCopy.front().first.second);
        for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
        {
            BondsCopy.at(CurrentBond).first = make_pair( BondsCopy.at(CurrentBond).first.first + shift.first, BondsCopy.at(CurrentBond).first.second + shift.second);
            BondsCopy.at(CurrentBond).second = make_pair( BondsCopy.at(CurrentBond).second.first + shift.first, BondsCopy.at(CurrentBond).second.second + shift.second);
            LocalGraphKey += BondsCopy.at(CurrentBond).first.first*this->Order + BondsCopy.at(CurrentBond).first.second;
            LocalGraphKey += BondsCopy.at(CurrentBond).second.first*this->Order + BondsCopy.at(CurrentBond).second.second;
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
    vector< pair<int, int> > SiteList;
    for(unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++)
    {
        SiteList.push_back( this->Bonds.at(CurrentBond).first );
        SiteList.push_back( this->Bonds.at(CurrentBond).second );
    }
    sort(SiteList.begin(), SiteList.end());
    vector< pair<int,int> >::iterator TrueEnd;
    TrueEnd = unique(SiteList.begin(), SiteList.end() );
    SiteList.resize( TrueEnd - SiteList.begin() );

    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++)
    {
        vector< pair<int, int> >::iterator FirstSite;
        vector< pair<int, int> >::iterator SecondSite;
        FirstSite = find(SiteList.begin(), SiteList.end(), this->Bonds.at(CurrentBond).first);
        SecondSite = find(SiteList.begin(), SiteList.end(), this->Bonds.at(CurrentBond).second);
        this->AdjacencyList.push_back(make_pair( FirstSite - SiteList.begin(), SecondSite - SiteList.begin() ) );
    }
    sort( this->AdjacencyList.begin(), this->AdjacencyList.end());
}

void ConstructSiteBasedGraphs(vector< vector< SiteGraph > > & graphs, int FinalOrder)
{
    vector< SiteGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    
    while (CurrentOrder <= FinalOrder)
    {
        
        NewGraphs.clear();
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
                    //NewGraph.SubgraphList.push_back(make_pair(1, OldGraph.Identifier) );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists = Exists || (NewGraph.Sites == NewGraphs.at(CurrentIndex).Sites ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraph.FindLatticeConstant();
                        NewGraph.GenerateAdjacencyList();
                        NewGraphs.push_back( NewGraph );
                    }
                
                }
                if( !binary_search( OldGraph.Sites.begin(), OldGraph.Sites.end(), NorthSite))
                {
                    SiteGraph NewGraph;
                    NewGraph = OldGraph;
                    NewGraph.AddSite(NorthSite.first, NorthSite.second);
                    NewGraph.Order = OldGraph.Order + 1;
                    //NewGraph.SubgraphList.push_back( make_pair(1, OldGraph.Identifier) );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists = Exists || (NewGraph.Sites == NewGraphs.at(CurrentIndex).Sites ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraph.FindLatticeConstant();
                        NewGraph.GenerateAdjacencyList();
                        NewGraphs.push_back(NewGraph);
                    }
                }
            }
        }
        
        graphs.insert(graphs.end(), NewGraphs);
        CurrentOrder++;
        
    }
}

void ConstructRectangularSiteGraphs(vector< vector< SiteGraph > > & graphs, unsigned int FinalWidth, unsigned int FinalHeight)
{
    graphs.resize(FinalWidth);
    int GlobalIdentifier = 0;

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalWidth; CurrentGraphWidth++)
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalHeight; CurrentGraphHeight++)
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            vector< pair<int, int> > SiteList;
            SiteList.resize(CurrentOrder);
            vector< pair<int, int> > Subgraphs;

            for( unsigned int CurrentCheckWidth = 1; CurrentCheckWidth <= CurrentGraphWidth; CurrentCheckWidth++)
            {
                for (unsigned int CurrentCheckHeight = 1; CurrentCheckHeight <= CurrentGraphHeight; CurrentCheckHeight++)
                {
                    SiteList.at(CurrentCheckHeight - 1 + (CurrentCheckWidth- 1)*CurrentGraphHeight) = make_pair(CurrentCheckWidth - 1, CurrentCheckHeight - 1);

                    int SubgraphCount = (CurrentGraphWidth + 1 - CurrentCheckWidth)*(CurrentGraphHeight + 1 - CurrentCheckHeight);
                    bool ThisGraph = (CurrentGraphHeight == CurrentCheckHeight) && (CurrentGraphWidth == CurrentCheckWidth);
                   
                    if( //CurrentCheckHeight <= graphs.at(CurrentCheckWidth - 1).size() && 
                        SubgraphCount > 0 &&
                        !ThisGraph
                        )
                    {
                        Subgraphs.push_back(make_pair( SubgraphCount, graphs.at(CurrentCheckWidth - 1).at(CurrentCheckHeight - 1).Identifier ) );
                    }
                }
            }

            SiteGraph NewGraph(SiteList, GlobalIdentifier++, CurrentOrder, 1, Subgraphs);
            NewGraph.GenerateAdjacencyList();
            graphs.at(CurrentGraphWidth - 1).push_back(NewGraph);
        }
    }
}

void ConstructRectangularSiteGraphs(vector< vector< SiteGraph > > & graphs, unsigned int FinalOrder)
{
    graphs.resize(FinalOrder);
    int GlobalIdentifier = 0;

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalOrder; CurrentGraphWidth++)
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalOrder && CurrentGraphHeight*CurrentGraphWidth <= FinalOrder; CurrentGraphHeight++)
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            vector< pair<int, int> > SiteList;
            SiteList.resize(CurrentOrder);
            vector< pair<int, int> > Subgraphs;

            for( unsigned int CurrentCheckWidth = 1; CurrentCheckWidth <= CurrentGraphWidth; CurrentCheckWidth++)
            {
                for (unsigned int CurrentCheckHeight = 1; CurrentCheckHeight <= CurrentGraphHeight; CurrentCheckHeight++)
                {
                    SiteList.at(CurrentCheckHeight - 1 + (CurrentCheckWidth- 1)*CurrentGraphHeight) = make_pair(CurrentCheckWidth - 1, CurrentCheckHeight - 1);

                    int SubgraphCount = (CurrentGraphWidth + 1 - CurrentCheckWidth)*(CurrentGraphHeight + 1 - CurrentCheckHeight);
                    bool ThisGraph = (CurrentGraphHeight == CurrentCheckHeight) && (CurrentGraphWidth == CurrentCheckWidth);
                   
                    if( //CurrentCheckHeight <= graphs.at(CurrentCheckWidth - 1).size() && 
                        SubgraphCount > 0 &&
                        !ThisGraph
                        )
                    {
                        Subgraphs.push_back(make_pair( SubgraphCount, graphs.at(CurrentCheckWidth - 1).at(CurrentCheckHeight - 1).Identifier ) );
                    }
                }
            }

            SiteGraph NewGraph(SiteList, GlobalIdentifier++, CurrentOrder, 1, Subgraphs);
            NewGraph.GenerateAdjacencyList();
            graphs.at(CurrentGraphWidth - 1).push_back(NewGraph);
        }
    }
}

void ConstructBondBasedGraphs(vector< vector< BondGraph > > & graphs, int FinalOrder)
{
    vector< BondGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    
    while (CurrentOrder <= FinalOrder)
    {
        NewGraphs.clear();
        for( unsigned int CurrentGraph = 0; CurrentGraph < graphs.back().size(); CurrentGraph++)
        {
            BondGraph OldGraph = graphs.back().at(CurrentGraph);
            for( unsigned int CurrentBond = 0; CurrentBond < OldGraph.Bonds.size(); CurrentBond++)
            {
                pair< pair<int, int>, pair<int, int> >& ThisBond = OldGraph.Bonds.at(CurrentBond);
                pair< pair<int, int>, pair<int, int> > FirstBond;
                pair< pair<int, int>, pair<int, int> > SecondBond;
                pair< pair<int, int>, pair<int, int> > ThirdBond;
                pair<int,int> NewSite;

                if( ThisBond.first.first != ThisBond.second.first ) //Bond is horizontal
                {
                    NewSite = make_pair(ThisBond.first.first, ThisBond.first.second + 1);
                    FirstBond = make_pair(ThisBond.first, NewSite);
                    NewSite = make_pair(ThisBond.second.first, ThisBond.second.second + 1);
                    SecondBond = make_pair(ThisBond.second, NewSite);
                    NewSite = make_pair(ThisBond.second.first + 1, ThisBond.second.second);
                    ThirdBond = make_pair(ThisBond.second, NewSite);
                }

                else //Bond is vertical
                {

                    NewSite = make_pair(ThisBond.second.first, ThisBond.second.second + 1);
                    FirstBond = make_pair(ThisBond.second, NewSite);
                    NewSite = make_pair(ThisBond.second.first + 1, ThisBond.second.second);
                    SecondBond = make_pair(ThisBond.second, NewSite);
                    NewSite = make_pair(ThisBond.first.first + 1, ThisBond.first.second);
                    ThirdBond = make_pair(ThisBond.first, NewSite);
                }
                 
                if( !binary_search( OldGraph.Bonds.begin(), OldGraph.Bonds.end(), FirstBond))
                {
                    BondGraph NewGraph;
                    NewGraph = OldGraph;
                    NewGraph.AddBond( FirstBond.first, FirstBond.second);
                    NewGraph.Order = OldGraph.Order + 1;
                    //NewGraph.SubgraphList.push_back(make_pair(1, OldGraph.Identifier) );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists = Exists || (NewGraph == NewGraphs.at(CurrentIndex) ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraphs.push_back( NewGraph );
                    }
                
                }
                if( !binary_search( OldGraph.Bonds.begin(), OldGraph.Bonds.end(), SecondBond))
                {
                    BondGraph NewGraph;
                    NewGraph = OldGraph;
                    NewGraph.AddBond(SecondBond.first, SecondBond.second);
                    NewGraph.Order = OldGraph.Order + 1;
                    //NewGraph.SubgraphList.push_back( make_pair(1, OldGraph.Identifier) );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists = Exists || (NewGraph == NewGraphs.at(CurrentIndex) ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraphs.push_back(NewGraph);
                    }
                }
                
                if( !binary_search( OldGraph.Bonds.begin(), OldGraph.Bonds.end(), ThirdBond))
                {
                    BondGraph NewGraph;
                    NewGraph = OldGraph;
                    NewGraph.AddBond(ThirdBond.first, ThirdBond.second);
                    NewGraph.Order = OldGraph.Order + 1;
                    //NewGraph.SubgraphList.push_back( make_pair(1, OldGraph.Identifier) );
                    NewGraph.MakeCanonical();
                    bool Exists = false;
                    for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                    {
                        Exists = Exists || (NewGraph == NewGraphs.at(CurrentIndex) ); 
                    }
                    if( !Exists )
                    {
                        NewGraph.Identifier = ++GlobalIdentifier;
                        NewGraphs.push_back(NewGraph);
                    }
                }
            }
        }
        graphs.insert(graphs.end(), NewGraphs);
        CurrentOrder++;
    }
}

void FindSubgraphs(vector< SiteGraph > & GraphList)
{
    for( unsigned int CurrentGraph = 1; CurrentGraph < GraphList.size(); CurrentGraph++) 
    {
        unsigned int CurrentCheck = 0;
        while (CurrentCheck < CurrentGraph && GraphList.at(CurrentCheck).Order < GraphList.at(CurrentGraph).Order)
        {
            vector< vector< pair<int,int> > > DistinctReps;
            DistinctReps.resize(GraphList.at(CurrentCheck).LatticeConstant);
            DistinctReps.push_back( GraphList.at(CurrentCheck).Sites );

            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
            {
                Dihedral Transform(CurrentElement);
                vector< pair<int,int> > ThisRep = GraphList.at(CurrentCheck).Sites;
                for_each(ThisRep.begin(), ThisRep.end(), Transform);
                sort(ThisRep.begin(), ThisRep.end());
                bool GlobalShifted = false;
                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                {
                    bool Shifted = true;
                    const pair< int, int> shift = make_pair(DistinctReps.at(CurrentRep).front().first - ThisRep.front().first, DistinctReps.at(CurrentRep).front().second - ThisRep.front().second);
            
                    unsigned int CurrentSite = 1;
                    while( Shifted && CurrentSite < DistinctReps.at(CurrentRep).size())
                    {
                        Shifted = Shifted && ((shift.first == (DistinctReps.at(CurrentRep).at(CurrentSite).first - ThisRep.at(CurrentSite).first)) && (shift.second == (DistinctReps.at(CurrentRep).at(CurrentSite).second - ThisRep.at(CurrentSite).second)));
                        CurrentSite++;
                    }
            
                    GlobalShifted = GlobalShifted || Shifted;
                }
                if (!GlobalShifted)
                {
                    const pair< int, int> shift = make_pair(-ThisRep.front().first, -ThisRep.front().second);
                    for( unsigned int CurrentSite = 0; CurrentSite < ThisRep.size(); CurrentSite++)
                    {
                        ThisRep.at(CurrentSite).first += shift.first;
                        ThisRep.at(CurrentSite).second += shift.second;
                    }
                    DistinctReps.push_back(ThisRep);
                }
            }

            //Now we have all distinct permutations of the graph!

            int Embeddings = 0;
            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
            {
                pair<int,int> shift = make_pair(0,0);
                for( int xBoost = 0; xBoost <= GraphList.at(CurrentGraph).Sites.back().first; xBoost++)
                {
                    for( int yBoost = 0; yBoost <= GraphList.at(CurrentGraph).Sites.back().second; yBoost++)
                    {
                        shift = make_pair(xBoost, yBoost);
                        vector< pair<int,int> > CheckList = DistinctReps.at(CurrentRep);
                        unsigned int Counter = 0;
                        for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps.at(CurrentRep).size(); CurrentSite++)
                        {
                            CheckList.at(CurrentSite).first += shift.first;
                            CheckList.at(CurrentSite).second += shift.second;
                            Counter += binary_search(GraphList.at(CurrentGraph).Sites.begin(), GraphList.at(CurrentGraph).Sites.end(), CheckList.at(CurrentSite));
                        }
                        if ( Counter == CheckList.size() )
                        {
                                    Embeddings++;
                        }    
                    }
                    if (Embeddings > 0)
                    {
                        GraphList.at(CurrentGraph).SubgraphList.push_back(make_pair(Embeddings, GraphList.at(CurrentCheck).Identifier));
                    }
                }
            }
            CurrentCheck++;
        }
    }
}

void FindSubgraphs(vector< vector< SiteGraph > > & GraphList)
{
    for( unsigned int CurrentGraphHeight = 0; CurrentGraphHeight < GraphList.size(); CurrentGraphHeight++) 
    {
        for( unsigned int CurrentGraphWidth = 0; CurrentGraphWidth < GraphList.at(CurrentGraphHeight).size(); CurrentGraphWidth++) 
        {
            for( unsigned int CurrentCheckHeight = 0; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
            {
            
                for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList.at(CurrentCheckHeight).size(); CurrentCheckWidth++ )
                {
                    vector< vector< pair<int,int> > > DistinctReps;
                    DistinctReps.push_back( GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Sites );

                    for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                    {
                         
                        Dihedral Transform(CurrentElement);
                        vector< pair<int,int> > ThisRep = GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Sites;
                        for_each(ThisRep.begin(), ThisRep.end(), Transform);
                        sort(ThisRep.begin(), ThisRep.end());
                        bool GlobalShifted = false;
                        for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                        {
                            bool Shifted = true;
                            const pair< int, int> shift = make_pair(DistinctReps.at(CurrentRep).front().first - ThisRep.front().first, DistinctReps.at(CurrentRep).front().second - ThisRep.front().second);
            
                            unsigned int CurrentSite = 1;
                            while( Shifted && CurrentSite < DistinctReps.at(CurrentRep).size())
                            {
                                Shifted = Shifted && ((shift.first == (DistinctReps.at(CurrentRep).at(CurrentSite).first - ThisRep.at(CurrentSite).first)) && (shift.second == (DistinctReps.at(CurrentRep).at(CurrentSite).second - ThisRep.at(CurrentSite).second)));
                                CurrentSite++;
                            }
            
                            GlobalShifted = GlobalShifted || Shifted;
                        }
                        if (!GlobalShifted)
                        {
                            const pair< int, int> shift = make_pair(-ThisRep.front().first, -ThisRep.front().second);
                            for( unsigned int CurrentSite = 0; CurrentSite < ThisRep.size(); CurrentSite++)
                            {
                                ThisRep.at(CurrentSite).first += shift.first;
                                ThisRep.at(CurrentSite).second += shift.second;
                            }
                            DistinctReps.push_back(ThisRep);
                        }
                    }

                    //Now we have all distinct permutations of the graph!

                    int Embeddings = 0;
                    for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                    {
                        pair<int,int> shift = make_pair(0,0);
                        for( int xBoost = 0; xBoost <= GraphList.at(CurrentGraphHeight).at(CurrentGraphWidth).Sites.back().first; xBoost++)
                        {
                            for( int yBoost = 0; yBoost <= GraphList.at(CurrentGraphHeight).at(CurrentGraphWidth).Sites.back().second; yBoost++)
                            {
                                shift = make_pair(xBoost, yBoost);
                                vector< pair<int,int> > CheckList = DistinctReps.at(CurrentRep);
                                unsigned int Counter = 0;
                                for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps.at(CurrentRep).size(); CurrentSite++)
                                {
                                    CheckList.at(CurrentSite).first += shift.first;
                                    CheckList.at(CurrentSite).second += shift.second;
                                    Counter += binary_search(GraphList.at(CurrentGraphHeight).at(CurrentGraphWidth).Sites.begin(), GraphList.at(CurrentGraphHeight).at(CurrentGraphWidth).Sites.end(), CheckList.at(CurrentSite));
                                }
                                if ( Counter == CheckList.size() )
                                {
                                    Embeddings++;
                                }
                            }
                        }
                    }
                    if (Embeddings > 0)
                    {
                        GraphList.at(CurrentGraphHeight).at(CurrentGraphWidth).SubgraphList.push_back(make_pair(Embeddings, GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Identifier));
                    }
                }
            }
        }
    }
}

void WriteGraphsToFile(vector<SiteGraph> & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++)
    {
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<0<<endl;

        for (unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentGraph).Sites.size(); CurrentSite++)
        {
            Output<<GraphList.at(CurrentGraph).Sites.at(CurrentSite).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).Sites.at(CurrentSite).second;
            Output<<" ";
        }
        Output<<endl;
        for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentGraph).AdjacencyList.size(); CurrentConnection++)
        {
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).second;
            Output<<" ";
        }
        Output<<endl;
        for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentGraph).SubgraphList.size(); CurrentSubgraph++)
        {
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
        }
        Output<<endl;

    }
}

void WriteGraphsToFile(vector< vector<SiteGraph> > & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++)
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<0<<endl;

            for (unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentWidth).at(CurrentHeight).Sites.size(); CurrentSite++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).second;
                Output<<" ";
            }
            Output<<endl;
            for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++)
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
void WriteGraphsToFile(vector<BondGraph> & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++)
    {
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<1<<endl;

        for (unsigned int CurrentBond = 0; CurrentBond < GraphList.at(CurrentGraph).Bonds.size(); CurrentBond++)
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
        for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentGraph).SubgraphList.size(); CurrentSubgraph++)
        {
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
        }
        Output<<endl;
    }
}

void WriteGraphsToFile(vector< vector<BondGraph> > & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++)
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<1<<endl;

            for (unsigned int CurrentBond = 0; CurrentBond < GraphList.at(CurrentWidth).at(CurrentHeight).Bonds.size(); CurrentBond++)
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
            for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
            } 
            Output<<endl;
        }
    }
}
