#include "graphs.h"

//! \file graphs.cpp The main source file for the Graphs project

/*int main()
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
    SiteGraph Start(SiteList, 0, 1, 1, Empty); 
    testsites[0].resize(1);
    testsites[0][0] = Start;
    for(unsigned int i = 1; i <= 8; i++)
    {
        ConstructSiteBasedGraphs(testsites, i);
        FindSubgraphs(testsites, i);
        WriteGraphsToFile(testsites, "8sitebased.dat", i);
    }
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
    BondGraph Start1(BondList1, 0, 0, 1, Empty1); 
    BondGraph Start2(BondList2, 1, 1, 2, Empty2);
    testbonds[0].resize(1);
    testbonds[0][0] = Start1;
    testbonds[0][0].LowField = true;
    testbonds[1].resize(1);
    testbonds[1][0] = Start2;
    testbonds[1][0].Identifier = 1;
    testbonds[1][0].LowField = true;
    testbonds[1][0].GenerateAdjacencyList();
    ConstructBondBasedGraphs(testbonds, 9);
    FindSubgraphs(testbonds);
    WriteGraphsToFile(testbonds, "order9bondbased.dat");
    for( unsigned int i = 0; i < testbonds.size(); i++)
    {
        for( unsigned int j = 0; j < testbonds.at(i).size(); j++)
        {
            testbonds.at(i).at(j).PrintGraph();
        }
    }
    return 0;

}*/

Graph::Graph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    SubgraphList.clear();
}

Graph::Graph(int IdentNumber, int Order, int LatticeConst, std::vector< std::pair<int, int> > & Subgraphs )
{
    Identifier = IdentNumber;
    Order = Order;
    LatticeConstant = LatticeConst;
    SubgraphList = Subgraphs;
}

Graph::Graph(int IdentNumber, int Order, int LatticeConst, std::vector< std::pair<int, int> > & Adjacency, std::vector< std::pair<int, int> > & Subgraphs )
{
    Identifier = IdentNumber;
    Order = Order;
    LatticeConstant = LatticeConst;
    AdjacencyList = Adjacency;
    SubgraphList = Subgraphs;
}

Graph& Graph::operator=( const Graph & Other)
{
    this->Order = other.Order;
    this->LatticeConstant = other.LatticeConstant;
    this->SubgraphList = other.SubgraphList;
    return *this;
}

bool Graph::operator==( const Graph & Other)
{
    return (( this->Order == other.Order) &&
           ( this->LatticeConstant == other.LatticeConstant) );
}

int Graph::Valency( int Site )
{
    int Count = 0;
    for( unsigned int CurrentConnection = 0; CurrentConnection < this->AdjacencyList.size(); CurrentConnection++)
    {
        if ( this->AdjacencyList[CurrentConnection].first == Site || this-> AdjacencyList[CurrentConnection].second == Site)
        {
            ++Count;
        }
    }
    return Count;
}

SiteGraph::SiteGraph()
{
    Order = 0;
    LatticeConstant = 1;
    Identifier = 0;
    Sites.clear();
    SubgraphList.clear();
}

SiteGraph::SiteGraph(std::vector<std::pair <int, int> > & SiteList, int IdentNumber, int SiteCount, int LatticeConst, std::vector< std::pair<int, int> > & Subgraphs )
{
    Identifier = IdentNumber;
    Order = SiteCount;
    LatticeConstant = LatticeConst;
    SubgraphList = Subgraphs;
    Sites = SiteList;
}

SiteGraph& SiteGraph::operator=( const SiteGraph & Other)
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
    this->Sites.insert( this->Sites.begin() + InsertCounter, std::make_pair(xIndex, yIndex));
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
    std::pair<int, int> TempSite = std::make_pair(xIndex, yIndex); 

    return std::binary_search(this->Sites.begin(), this->Sites.end(), TempSite);
}

void SiteGraph::PrintGraph()
{
    cout<<"ID number: "<<this->Identifier<<endl;
    cout<<"Order: "<<this->Order<<endl;
    cout<<"Lattice Constant: "<<this->LatticeConstant<<endl;
    cout<<"Real space coordinates: "<<endl;
    for( unsigned int CurrentSite = 0; CurrentSite < this->Sites.size(); CurrentSite++)
    {
        cout<<"("<<this->Sites.at(CurrentSite).first<<", "<<this->Sites.at(CurrentSite).second<<") ";
    }
    cout<<endl;
    cout<<"Subgraph List: "<<endl;
    for ( unsigned int CurrentSubgraph = 0; CurrentSubgraph < this->SubgraphList.size(); CurrentSubgraph++)
    {
        cout<<"("<<this->SubgraphList.at(CurrentSubgraph).first<<", "<<this->SubgraphList.at(CurrentSubgraph).second<<") ";
    }
    cout<<endl;
    cout<<"Adjacency List: "<<endl;
    for ( unsigned int CurrentBond = 0; CurrentBond < this->AdjacencyList.size(); CurrentBond++)
    {
        cout<<"("<<this->AdjacencyList.at(CurrentBond).first<<", "<<this->AdjacencyList.at(CurrentBond).second<<") ";
    }
    cout<<endl;
}

void SiteGraph::FindLatticeConstant()
{
    //! [Site lattice constant copy]
    std::vector< std::vector< std::pair<int,int> > > SiteLists;
    SiteLists.push_back(this->Sites);
    int Counter = 1;
    //! [Site lattice constant copy]
    //! [Site lattice constant dihedral transform]
    for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++ )
    {
        std::vector< std::pair<int,int> > TempSites = this->Sites;
        Dihedral Transform(CurrentElement);
        std::for_each(TempSites.begin(), TempSites.end(), Transform);
        std::sort(TempSites.begin(), TempSites.end());
        //! [Site lattice constant dihedral transform]
        //! [Site lattice constant shift check]
        bool GlobalShifted = false;
        for ( unsigned int CurrentList = 0; CurrentList < SiteLists.size(); CurrentList++ )
        {
            bool Shifted = true;
            const std::pair< int, int> shift = std::make_pair(SiteLists.at(CurrentList).front().first - TempSites.front().first, SiteLists.at(CurrentList).front().second - TempSites.front().second);
            
            unsigned int CurrentSite = 1;
            while( Shifted && CurrentSite < SiteLists.at(CurrentList).size())
            {
                Shifted = Shifted && ((shift.first == (SiteLists.at(CurrentList).at(CurrentSite).first - TempSites.at(CurrentSite).first)) && (shift.second == (SiteLists.at(CurrentList).at(CurrentSite).second - TempSites.at(CurrentSite).second)));
                CurrentSite++;
            }
            
            GlobalShifted = GlobalShifted || Shifted;
        }
        //! [Site lattice constant shift check]
        //! [Site lattice constant new rep]
        if (!GlobalShifted)
        {
            SiteLists.push_back(TempSites);
            Counter++;
        }
        //! [Site lattice constant new rep]
    }

    this->LatticeConstant = Counter;
}

Dihedral::Dihedral()
{
    element = 0;
}

Dihedral::Dihedral(int Factor)
{
    element = Factor;
}

void Dihedral::operator()(std::pair<int, int> & Coordinates)
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

void Dihedral::operator()(std::pair< std::pair<int, int>, std::pair<int,int> > & Coordinates)
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

bool SiteGraph::operator==(const SiteGraph & Other)
{
    if( ( other.LatticeConstant == this->LatticeConstant) &&
        ( other.Order == this->Order) )
    {
        for( int currentFactor = 0; currentFactor < 8; currentFactor++)
        {
            //! [Site isomorphism copy and transform]
            std::vector< std::pair< int, int> > SitesCopy = other.Sites;
            Dihedral Transform(currentFactor);
            std::for_each(SitesCopy.begin(), SitesCopy.end(), Transform);
            std::sort(SitesCopy.begin(), SitesCopy.end());
            //! [Site isomorphism copy and transform]
            //! [Site isomorphism shift check]
            bool Isomorphic = true; 
            const std::pair<int,int> shift = std::make_pair(this->Sites.front().first - SitesCopy.front().first, this->Sites.front().second - SitesCopy.front().second);
            unsigned int CurrentSite = 0;
            while(Isomorphic && CurrentSite < SitesCopy.size())
            {
                Isomorphic = (Isomorphic) && (shift.first == (this->Sites.at(CurrentSite).first - SitesCopy.at(CurrentSite).first)) && (shift.second == (this->Sites.at(CurrentSite).second - SitesCopy.at(CurrentSite).second ));
                CurrentSite++;
            }
            if (this->AdjacencyList.size() > 0 && other.AdjacencyList.size() > 0)
            {
                Isomorphic = Isomorphic || (this->AdjacencyList == other.AdjacencyList);
            }
            if (Isomorphic)
            {
                return true;
            }
            //! [Site isomorphism shift check]
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
    //! [Creating site canonical storage]
    std::vector< std::pair<int,int> > CanonicalSites;
    //! [Creating site canonical storage]
    //! [Transforming site canonical with Dihedral]
    for( int currentFactor = 0; currentFactor < 8; currentFactor++)
    {
        std::vector< std::pair<int,int> > SitesCopy = this->Sites;

        Dihedral Transform(currentFactor);
        std::for_each(SitesCopy.begin(), SitesCopy.end(), Transform);
        std::sort(SitesCopy.begin(), SitesCopy.end());
        //! [Transforming site canonical with Dihedral]
        //! [Shifting site canonical]
        std::pair<int,int> shift = std::make_pair(-SitesCopy.front().first, -SitesCopy.front().second);
        for(unsigned int CurrentSite = 0; CurrentSite < SitesCopy.size(); CurrentSite++)
        {
            SitesCopy.at(CurrentSite) = std::make_pair(SitesCopy.at(CurrentSite).first + shift.first, SitesCopy.at(CurrentSite).second + shift.second);
        }
        
        if( lexicographical_compare(CanonicalSites.begin(), CanonicalSites.end(), SitesCopy.begin(), SitesCopy.end() ) )
        {
            CanonicalSites = SitesCopy;
        }
        //! [Shifting site canonical]
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
                AdjacencyList.push_back(std::make_pair(CurrentSite, CurrentIndex) );
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

BondGraph::BondGraph(std::vector< std::pair< std::pair <int, int>, std::pair<int, int> > > & BondList, int IdentNumber, int Order, int LatticeConst, std::vector< std::pair< int, int > > & Subgraphs )
{
    Identifier = IdentNumber;
    Order = Order;
    LatticeConstant = LatticeConst;
    SubgraphList = Subgraphs;
    Bonds = BondList;
}

BondGraph& BondGraph::operator=( const BondGraph & Other)
{
    this->Order = other.Order;
    this->LatticeConstant = other.LatticeConstant;
    this->SubgraphList = other.SubgraphList;
    this->Bonds = other.Bonds;
    return *this;
}

bool BondGraph::operator==( const BondGraph & Other)
{
    if( (other.Order == this->Order) )
    {
        for( int currentFactor = 0; currentFactor < 8; currentFactor++)
        {
            //! [Bond isomorphism copy and transform]
            std::vector< std::pair< std::pair< int, int>, std::pair< int, int> > > BondsCopy = other.Bonds;
            Dihedral Transform(currentFactor);
            std::for_each(BondsCopy.begin(), BondsCopy.end(), Transform);
            for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
            {
                std::pair<int,int> Temp = BondsCopy.at(CurrentBond).first;
                if( BondsCopy.at(CurrentBond).first > BondsCopy.at(CurrentBond).second)
                {
                    BondsCopy.at(CurrentBond).first = BondsCopy.at(CurrentBond).second;
                    BondsCopy.at(CurrentBond).second = Temp;
                }
            }
            std::sort(BondsCopy.begin(), BondsCopy.end());
            //! [Bond isomorphism copy and transform]
            //! [Bond isomorphism shift check]
            std::pair<int, int> shift1 = std::make_pair(this->Bonds.front().first.first - BondsCopy.front().first.first, this->Bonds.front().first.second - BondsCopy.front().first.second);
            std::pair<int, int> shift2 = std::make_pair(this->Bonds.front().second.first - BondsCopy.front().second.first, this->Bonds.front().second.second - BondsCopy.front().second.second);
            bool Isomorphic = shift1 == shift2;
            unsigned int CurrentBond = 0;
            while( Isomorphic && CurrentBond < BondsCopy.size())
            {
                Isomorphic = Isomorphic && (shift1 == std::make_pair(this->Bonds.at(CurrentBond).first.first - BondsCopy.at(CurrentBond).first.first, this->Bonds.at(CurrentBond).first.second - BondsCopy.at(CurrentBond).first.second));
                Isomorphic = Isomorphic && (shift2 == std::make_pair(this->Bonds.at(CurrentBond).second.first - BondsCopy.at(CurrentBond).second.first, this->Bonds.at(CurrentBond).second.second - BondsCopy.at(CurrentBond).second.second));
                CurrentBond++;
            }
            if (Isomorphic)
            {
                return true;
            }
            //! [Bond isomorphism shift check]
        }
        return false;
    }
    else
    {
        return false;
    }
}

void BondGraph::AddBond(std::pair<int, int> FirstSite, std::pair<int, int> SecondSite )
{
    unsigned int InsertCounter = 0;
    while ( InsertCounter < this->Bonds.size() && this->Bonds.at(InsertCounter).first <= FirstSite && this->Bonds.at(InsertCounter).second <= SecondSite )
    {
        InsertCounter++;
    }
    this->Bonds.insert( this->Bonds.begin() + InsertCounter, std::make_pair(FirstSite, SecondSite));
}

void BondGraph::RemoveBond( std::pair<int, int> FirstSite, std::pair<int, int> SecondSite)
{
    unsigned int EraseCounter = 0;
    while ( this->Bonds.at(EraseCounter).first <= FirstSite && this->Bonds.at(EraseCounter).second <= SecondSite && EraseCounter< this->Bonds.size())
    {
        EraseCounter++;
    }
    this->Bonds.erase( this->Bonds.begin() + EraseCounter);
}

bool BondGraph::CheckForBond(std::pair<int,int> FirstSite, std::pair<int,int> SecondSite)
{
    std::pair< std::pair< int, int>, std::pair< int, int> > TempBond = std::make_pair(FirstSite, SecondSite); 

    return std::binary_search(this->Bonds.begin(), this->Bonds.end(), TempBond);
}

int BondGraph::BondCount(std::pair<int,int> FirstSite, std::pair<int,int> SecondSite)
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
    //! [Bond canonical storage]
    std::vector<std::pair< std::pair<int,int>, std::pair<int,int> > > CanonicalBonds;
    //! [Bond canonical storage]
    //! [Bond canonical transform] 
    for( int currentFactor = 0; currentFactor < 8; currentFactor++)
    {
        int LocalGraphKey = 0;
        std::vector< std::pair< std::pair< int, int>, std::pair<int,int> > > BondsCopy = this->Bonds;
        Dihedral Transform(currentFactor);
        std::for_each(BondsCopy.begin(), BondsCopy.end(), Transform);
        for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
        {
            std::pair<int,int> Temp = BondsCopy.at(CurrentBond).first;
            if( BondsCopy.at(CurrentBond).first > BondsCopy.at(CurrentBond).second)
            {
                BondsCopy.at(CurrentBond).first = BondsCopy.at(CurrentBond).second;
                BondsCopy.at(CurrentBond).second = Temp;
            }
        }
        std::sort(BondsCopy.begin(), BondsCopy.end());
        //! [Bond canonical transform]
        //! [Bond canonical shift]
        std::pair<int,int> shift = std::make_pair(-BondsCopy.front().first.first, -BondsCopy.front().first.second);
        for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
        {
            BondsCopy.at(CurrentBond).first = std::make_pair( BondsCopy.at(CurrentBond).first.first + shift.first, BondsCopy.at(CurrentBond).first.second + shift.second);
            BondsCopy.at(CurrentBond).second = std::make_pair( BondsCopy.at(CurrentBond).second.first + shift.first, BondsCopy.at(CurrentBond).second.second + shift.second);
            LocalGraphKey += BondsCopy.at(CurrentBond).first.first*this->Order + BondsCopy.at(CurrentBond).first.second;
            LocalGraphKey += BondsCopy.at(CurrentBond).second.first*this->Order + BondsCopy.at(CurrentBond).second.second;
        }
        if( LocalGraphKey > GlobalGraphKey )
        {
            GlobalGraphKey = LocalGraphKey;
            CanonicalBonds = BondsCopy;
        }
        //! [Bond canonical shift]
    }
    this->Bonds = CanonicalBonds;
}

void BondGraph::GenerateAdjacencyList()
{
    //! [Bond adjacency copy]
    std::vector< std::pair<int, int> > SiteList;
    for(unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++)
    {
        SiteList.push_back( this->Bonds.at(CurrentBond).first );
        SiteList.push_back( this->Bonds.at(CurrentBond).second );
    }
    std::sort(SiteList.begin(), SiteList.end());
    std::vector< std::pair<int,int> >::iterator TrueEnd;
    TrueEnd = unique(SiteList.begin(), SiteList.end() );
    SiteList.resize( TrueEnd - SiteList.begin() );
    this->AdjacencyList.resize(this->Bonds.size());
    //! [Bond adjacency copy]
    //! [Bond adjacency fill list]
    for( unsigned int CurrentBond = 0; CurrentBond < this->Bonds.size(); CurrentBond++)
    {
        std::vector< std::pair<int, int> >::iterator FirstSite;
        std::vector< std::pair<int, int> >::iterator SecondSite;
        FirstSite = std::find(SiteList.begin(), SiteList.end(), this->Bonds.at(CurrentBond).first);
        SecondSite = std::find(SiteList.begin(), SiteList.end(), this->Bonds.at(CurrentBond).second);
        this->AdjacencyList[CurrentBond] = std::make_pair( FirstSite - SiteList.begin(), SecondSite - SiteList.begin() ) ;
    }
    //! [Bond adjacency fill list]
    std::sort( this->AdjacencyList.begin(), this->AdjacencyList.end());
}

void BondGraph::FindLatticeConstant()
{
    //! [Bond lattice constant copy]
    std::vector< std::vector< std::pair< std::pair<int, int >, std::pair<int,int> > > > BondLists;
    BondLists.push_back(this->Bonds);
    int Counter = 1;
    //! [Bond lattice constant copy]
    //! [Bond lattice constant transform]
    for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++ )
    {
        std::vector< std::pair< std::pair<int, int>, std::pair<int,int> > > TempBonds = this->Bonds;
        Dihedral Transform(CurrentElement);
        std::for_each(TempBonds.begin(), TempBonds.end(), Transform);
        for(unsigned int CurrentBond = 0; CurrentBond < TempBonds.size(); CurrentBond++)
        {
            std::pair<int,int> Temp = TempBonds.at(CurrentBond).first;
            if( TempBonds.at(CurrentBond).first > TempBonds.at(CurrentBond).second)
            {
                TempBonds.at(CurrentBond).first = TempBonds.at(CurrentBond).second;
                TempBonds.at(CurrentBond).second = Temp;
            }
        }
        std::sort(TempBonds.begin(), TempBonds.end());
        //! [Bond lattice constant transform]
        //! [Bond lattice constant shift check]
        bool GlobalShifted = false;
        for ( unsigned int CurrentList = 0; CurrentList < BondLists.size(); CurrentList++ )
        {
            const std::pair< int, int> shift1 = std::make_pair(BondLists.at(CurrentList).front().first.first - TempBonds.front().first.first, BondLists.at(CurrentList).front().first.second - TempBonds.front().first.second);
            const std::pair< int, int> shift2 = std::make_pair(BondLists.at(CurrentList).front().second.first - TempBonds.front().second.first, BondLists.at(CurrentList).front().second.second - TempBonds.front().second.second);
            bool Shifted = (shift1 == shift2); 
            unsigned int CurrentBond = 1;
            while( Shifted && CurrentBond < BondLists.at(CurrentList).size())
            {
                Shifted = Shifted && ((shift1.first == (BondLists.at(CurrentList).at(CurrentBond).first.first - TempBonds.at(CurrentBond).first.first)) && (shift1.second == (BondLists.at(CurrentList).at(CurrentBond).first.second - TempBonds.at(CurrentBond).first.second)));
                Shifted = Shifted && ((shift2.first == (BondLists.at(CurrentList).at(CurrentBond).second.first - TempBonds.at(CurrentBond).second.first)) && (shift2.second == (BondLists.at(CurrentList).at(CurrentBond).second.second - TempBonds.at(CurrentBond).second.second)));
                CurrentBond++;
            }
            
            GlobalShifted = GlobalShifted || Shifted;
        }
        //! [Bond lattice constant shift check]
        //! [Bond lattice constant store]
        if (!GlobalShifted)
        {
            BondLists.push_back(TempBonds);
            Counter++;
        }
        //! [Bond lattice constant store]
    }

    this->LatticeConstant = Counter;

}

int BondGraph::NumberSites()
{
    int max = 0;
    for( unsigned int CurrentBond = 0; CurrentBond < this->AdjacencyList.size(); CurrentBond++)
    {
        max = (this->AdjacencyList[CurrentBond].first > max) ? this->AdjacencyList[CurrentBond].first : max;
        max = (this->AdjacencyList[CurrentBond].second > max) ? this->AdjacencyList[CurrentBond].second : max;
    }
    max++;
    return max;
}

void ConstructSiteBasedGraphs(std::vector< std::vector< SiteGraph > > & graphs, int FinalOrder)
{
    std::vector< SiteGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    
    while (CurrentOrder <= FinalOrder)
    {
        cout<<"Current Order: "<<CurrentOrder<<" Current Global ID: "<<GlobalIdentifier<<endl;   
        NewGraphs.clear();
        for( unsigned int CurrentGraph = 0; CurrentGraph < graphs.back().size(); CurrentGraph++)
        {
            int tid;
            std::pair<int,int> NewSite;
            SiteGraph OldGraph = graphs.back().at(CurrentGraph);
            SiteGraph NewGraph;
            omp_set_num_threads( CurrentOrder - 1 );
            #pragma omp parallel private(tid, NewSite, NewGraph) shared(OldGraph, NewGraphs, GlobalIdentifier)
            {   
                tid = omp_get_thread_num();
                for( int i = 0; i < 4; i++ )
                {
                    switch ( i )
                    {
                        case 0 :
                            NewSite = std::make_pair( OldGraph.Sites.at( tid ).first + 1, OldGraph.Sites.at( tid ).second);
                            break;
                        case 1 :
                            NewSite = std::make_pair( OldGraph.Sites.at( tid ).first, OldGraph.Sites.at( tid ).second + 1);
                            break;
                        case 2 :
                            NewSite = std::make_pair( OldGraph.Sites.at( tid ).first - 1, OldGraph.Sites.at( tid ).second);
                            break;
                        case 3 :
                            NewSite = std::make_pair( OldGraph.Sites.at( tid ).first , OldGraph.Sites.at( tid ).second - 1);
                            break;
                    }
            
                    if( !std::binary_search( OldGraph.Sites.begin(), OldGraph.Sites.end(), NewSite))
                    {
                        NewGraph = OldGraph;
                        NewGraph.AddSite( NewSite.first, NewSite.second);
                        NewGraph.Order = OldGraph.Order + 1;
                        NewGraph.MakeCanonical();
                        NewGraph.GenerateAdjacencyList();
                        bool Exists = false;
                        
                        #pragma omp critical
                        {
                            for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                            {
                                Exists = Exists || (NewGraph.Sites == NewGraphs.at(CurrentIndex).Sites );
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
        graphs.insert(graphs.end(), NewGraphs);
        CurrentOrder++;
    }
}

void ConstructRectangularSiteGraphs(std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalWidth, unsigned int FinalHeight)
{
    graphs.resize(FinalWidth);
    int GlobalIdentifier = 0;

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalWidth; CurrentGraphWidth++)
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalHeight; CurrentGraphHeight++)
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            std::vector< std::pair<int, int> > SiteList;
            SiteList.resize(CurrentOrder);
            std::vector< std::pair<int, int> > Subgraphs;

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
            NewGraph.LowField = false;
            graphs.at(CurrentGraphWidth - 1).push_back(NewGraph);
        }
    }
}

void ConstructRectangularSiteGraphs(std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalOrder)
{
    graphs.resize(FinalOrder);
    int GlobalIdentifier = 0;

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalOrder; CurrentGraphWidth++)
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalOrder && CurrentGraphHeight*CurrentGraphWidth <= FinalOrder; CurrentGraphHeight++)
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            std::vector< std::pair<int, int> > SiteList;
            SiteList.resize(CurrentOrder);
            std::vector< std::pair<int, int> > Subgraphs;

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
            NewGraph.LowField = false;
            graphs.at(CurrentGraphWidth - 1).push_back(NewGraph);
        }
    }
}

void ConstructBondBasedGraphs(std::vector< std::vector< BondGraph > > & graphs, int FinalOrder)
{
    std::vector< BondGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    while (CurrentOrder <= FinalOrder)
    {
        cout<<CurrentOrder<<endl;
        NewGraphs.clear();
        for( unsigned int CurrentGraph = 0; CurrentGraph < graphs.back().size(); CurrentGraph++)
        {
            BondGraph OldGraph = graphs.back().at(CurrentGraph);
            for( unsigned int CurrentBond = 0; CurrentBond < OldGraph.Bonds.size(); CurrentBond++)
            {
                std::pair< std::pair< int, int>, std::pair< int, int> > NewBond;
                std::pair< int, int> NewSite;
                BondGraph NewGraph;

                int tid;
                #pragma omp parallel private(tid, NewSite, NewBond, NewGraph) shared(OldGraph, NewGraphs, GlobalIdentifier) num_threads(OldGraph.Bonds.size())
                {
                    tid = omp_get_thread_num();
                    for( int i = 0; i < 6; i++)
                    {
                            
                        switch( i )
                        {
                            case 0 :
                                if( OldGraph.Bonds.at( tid ).first.first != OldGraph.Bonds.at( tid ).second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).first.first, OldGraph.Bonds.at( tid ).first.second + 1);
                                    NewBond = std::make_pair(OldGraph.Bonds.at( tid ).first, NewSite);
                                } //Add a bond on the left, pointing up
                                else 
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).second.first, OldGraph.Bonds.at( tid ).second.second + 1);
                                    NewBond = std::make_pair(OldGraph.Bonds.at( tid ).second, NewSite);
                                } // Add a bond on the top, pointing up
                                break;
                            case 1 :
                            
                                if( OldGraph.Bonds.at( tid ).first.first != OldGraph.Bonds.at( tid ).second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).second.first, OldGraph.Bonds.at( tid ).second.second + 1);
                                    NewBond = std::make_pair(OldGraph.Bonds.at( tid ).second, NewSite);
                                } //Add a bond on the right, pointing up
                                else 
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).second.first + 1, OldGraph.Bonds.at( tid ).second.second);
                                    NewBond = std::make_pair(OldGraph.Bonds.at( tid ).second, NewSite);
                                } // Add a bond on top, pointing right
                                break;
                            case 2 :
                            
                                if( OldGraph.Bonds.at( tid ).first.first != OldGraph.Bonds.at( tid ).second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).second.first + 1, OldGraph.Bonds.at( tid ).second.second);
                                    NewBond = std::make_pair(OldGraph.Bonds.at( tid ).second, NewSite);
                                } // Add a bond on the right, pointing right
                                else 
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).first.first + 1, OldGraph.Bonds.at( tid ).first.second);
                                    NewBond = std::make_pair(OldGraph.Bonds.at( tid ).first, NewSite);
                                } // Add a bond on the bottom, pointing right
                                break;
                            case 3 : 
                            
                                if( OldGraph.Bonds.at( tid ).first.first != OldGraph.Bonds.at( tid ).second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).first.first - 1, OldGraph.Bonds.at( tid ).first.second);
                                    NewBond = std::make_pair(NewSite, OldGraph.Bonds.at( tid ).first);
                                } // Add a bond on the left, pointing left
                                else 
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).second.first - 1, OldGraph.Bonds.at( tid ).second.second);
                                    NewBond = std::make_pair(NewSite, OldGraph.Bonds.at( tid ).second);
                                } // Add a bond on the top, pointing left
                                break;
                            case 4 : 
                            
                                if( OldGraph.Bonds.at( tid ).first.first != OldGraph.Bonds.at( tid ).second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).first.first, OldGraph.Bonds.at( tid ).first.second - 1);
                                    NewBond = std::make_pair(NewSite, OldGraph.Bonds.at( tid ).first);
                                } // Add a bond to the left, pointing down
                                else 
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).first.first - 1, OldGraph.Bonds.at( tid ).first.second);
                                    NewBond = std::make_pair(NewSite, OldGraph.Bonds.at( tid ).first);
                                } // Add a bond to the bottom, pointing left
                                break;
                            case 5 : 
                            
                                if( OldGraph.Bonds.at( tid ).first.first != OldGraph.Bonds.at( tid ).second.first ) //Bond is horizontal
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).second.first, OldGraph.Bonds.at( tid ).second.second - 1);
                                    NewBond = std::make_pair(NewSite, OldGraph.Bonds.at( tid ).second);
                                } // Add a bond to the right, pointing down
                                else 
                                {
                                    NewSite = std::make_pair(OldGraph.Bonds.at( tid ).first.first, OldGraph.Bonds.at( tid ).first.second - 1);
                                    NewBond = std::make_pair(NewSite, OldGraph.Bonds.at( tid ).first);
                                } // Add a bond to the bottom, pointing down
                                break;

                        }
                    
                        if( !std::binary_search( OldGraph.Bonds.begin(), OldGraph.Bonds.end(), NewBond))
                        {
                            NewGraph = OldGraph;
                            NewGraph.AddBond( NewBond.first, NewBond.second);
                            NewGraph.Order = OldGraph.Order + 1;
                            NewGraph.MakeCanonical();
                            bool Exists = false;
                            #pragma omp critical
                            {
                                for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                                {
                                    Exists = Exists || (NewGraph == NewGraphs.at(CurrentIndex) ); 
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
        graphs.insert(graphs.end(), NewGraphs);
        CurrentOrder++;
    }
}

void FindSubgraphs(std::vector< SiteGraph > & GraphList)
{
    int tid;
    int gid;
    int MaxSize = omp_get_num_procs();
    for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.size(); CurrentGraphGroup++)
    {
        #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup) num_threads( MaxSize )
        {
            tid = omp_get_thread_num();
            gid = tid + CurrentGraphGroup * MaxSize;
            if ( (unsigned int) gid < GraphList.size() )
            {
                GraphList.at( gid ).SubgraphList.push_back(std::make_pair(GraphList.at( gid ).Order, 0));
    
                unsigned int CurrentCheck = 1;
                while (CurrentCheck < (unsigned int) gid && GraphList.at(CurrentCheck).Order < GraphList.at( gid ).Order)
                {
                    std::vector< std::vector< std::pair< int, int> > > DistinctReps;
                    DistinctReps.resize(GraphList.at(CurrentCheck).LatticeConstant);
                    DistinctReps.push_back( GraphList.at(CurrentCheck).Sites );

                    for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                    {
                        Dihedral Transform(CurrentElement);
                        std::vector< std::pair< int, int> > ThisRep = GraphList.at(CurrentCheck).Sites;
                        std::for_each(ThisRep.begin(), ThisRep.end(), Transform);
                        std::sort(ThisRep.begin(), ThisRep.end());
                        bool GlobalShifted = false;
                        for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                        {
                            bool Shifted = true;
                            const std::pair< int, int> shift = std::make_pair(DistinctReps.at(CurrentRep).front().first - ThisRep.front().first, DistinctReps.at(CurrentRep).front().second - ThisRep.front().second);
            
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
                            const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
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
                        std::pair< int, int> shift = std::make_pair(0,0);
                        int xMax = 0;
                        int yMax = 0;
                        int xMin = 0;
                        int yMin = 0;
                        for( unsigned int CurrentElement = 0; CurrentElement < GraphList.at( gid ).Sites.size(); CurrentElement++)
                        {
                            xMax = (GraphList.at( gid ).Sites.at(CurrentElement).first > xMax) ? GraphList.at( gid ).Sites.at(CurrentElement).first : xMax;
                            yMax = (GraphList.at( gid ).Sites.at(CurrentElement).second > yMax) ? GraphList.at( gid ).Sites.at(CurrentElement).second : yMax;
                            xMin = (GraphList.at( gid ).Sites.at(CurrentElement).first < xMin) ? GraphList.at( gid ).Sites.at(CurrentElement).first : xMin;
                            yMin = (GraphList.at( gid ).Sites.at(CurrentElement).second < yMin) ? GraphList.at( gid ).Sites.at(CurrentElement).second : yMin;
                        }

                        for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                        {
                            for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                            {
                                shift = std::make_pair(xBoost, yBoost);
                                std::vector< std::pair< int, int> > CheckList = DistinctReps.at(CurrentRep);
                                unsigned int Counter = 0;
                                for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps.at(CurrentRep).size(); CurrentSite++)
                                {
                                    CheckList.at(CurrentSite).first += shift.first;
                                    CheckList.at(CurrentSite).second += shift.second;
                                    Counter += std::binary_search(GraphList.at( gid ).Sites.begin(), GraphList.at( gid ).Sites.end(), CheckList.at(CurrentSite));
                                }
                                if ( Counter == CheckList.size() )
                                {
                                            Embeddings++;
                                }    
                            }
                            if (Embeddings > 0)
                            {
                                GraphList.at( gid ).SubgraphList.push_back(make_pair(Embeddings, GraphList.at(CurrentCheck).Identifier));
                            }
                        }
                    }
                    CurrentCheck++;
                }
            }
        }
    }
}

void FindSubgraphs(std::vector< std::vector< SiteGraph > > & GraphList)
{
    for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight < GraphList.size(); CurrentGraphHeight++) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.at(CurrentGraphHeight).size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                if( (unsigned int) gid < GraphList.at(CurrentGraphHeight).size() )
                {

                    GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back( std::make_pair(GraphList.at(CurrentGraphHeight).at( gid ).Order, 0) );
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList.at(CurrentCheckHeight).size(); CurrentCheckWidth++ )
                        {
                            std::vector< std::vector< std::pair< int, int> > > DistinctReps;
                            DistinctReps.push_back( GraphList.at(CurrentCheckHeight).at( CurrentCheckWidth ).Sites );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                         
                                Dihedral Transform(CurrentElement);
                                std::vector< std::pair< int, int> > ThisRep = GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Sites;
                                std::for_each( ThisRep.begin(), ThisRep.end(), Transform );
                                std::sort( ThisRep.begin(), ThisRep.end() );
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    bool Shifted = true;
                                    const std::pair< int, int> shift = make_pair(DistinctReps.at(CurrentRep).front().first - ThisRep.front().first, DistinctReps.at(CurrentRep).front().second - ThisRep.front().second);
            
                                    unsigned int CurrentSite = 1;
                                    while( Shifted && CurrentSite < DistinctReps.at(CurrentRep).size())
                                    {
                                        Shifted = Shifted && ((shift.first == (DistinctReps.at(CurrentRep).at(CurrentSite).first - ThisRep.at(CurrentSite).first)) && (shift.second == (DistinctReps.at(CurrentRep).at(CurrentSite).second - ThisRep.at(CurrentSite).second)));
                                        CurrentSite++;
                                    }
            
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if ( !GlobalShifted )
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
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
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList.at(CurrentGraphHeight).at( gid ).Sites.size(); CurrentElement++)
                                {
                                    xMax = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first > xMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first : xMax;
                                    yMax = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second > yMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second : yMax;
                                    xMin = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first < xMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first : xMin;
                                    yMin = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second < yMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        std::vector< std::pair< int, int> > CheckList = DistinctReps.at(CurrentRep);
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps.at(CurrentRep).size(); CurrentSite++)
                                        {
                                            CheckList.at(CurrentSite).first += shift.first;
                                            CheckList.at(CurrentSite).second += shift.second;
                                            Counter += std::binary_search(GraphList.at(CurrentGraphHeight).at( gid ).Sites.begin(), GraphList.at(CurrentGraphHeight).at( gid ).Sites.end(), CheckList.at(CurrentSite));
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
                                GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back(make_pair(Embeddings, GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Identifier));
                            }
                        }
                    }   
                }
            }
        }
    }
}

void FindSubgraphs(std::vector< std::vector< SiteGraph > > & GraphList, unsigned int Index)
{
    unsigned int CurrentGraphHeight = Index - 1;
    if ( CurrentGraphHeight < GraphList.size() ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.at(CurrentGraphHeight).size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                if( (unsigned int) gid < GraphList.at(CurrentGraphHeight).size() )
                {

                    GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back( std::make_pair(GraphList.at(CurrentGraphHeight).at( gid ).Order, 0) );
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList.at(CurrentCheckHeight).size(); CurrentCheckWidth++ )
                        {
                            std::vector< std::vector< std::pair< int, int> > > DistinctReps;
                            DistinctReps.push_back( GraphList.at(CurrentCheckHeight).at( CurrentCheckWidth ).Sites );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                         
                                Dihedral Transform(CurrentElement);
                                std::vector< std::pair< int, int> > ThisRep = GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Sites;
                                std::for_each( ThisRep.begin(), ThisRep.end(), Transform );
                                std::sort( ThisRep.begin(), ThisRep.end() );
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    bool Shifted = true;
                                    const std::pair< int, int> shift = make_pair(DistinctReps.at(CurrentRep).front().first - ThisRep.front().first, DistinctReps.at(CurrentRep).front().second - ThisRep.front().second);
            
                                    unsigned int CurrentSite = 1;
                                    while( Shifted && CurrentSite < DistinctReps.at(CurrentRep).size())
                                    {
                                        Shifted = Shifted && ((shift.first == (DistinctReps.at(CurrentRep).at(CurrentSite).first - ThisRep.at(CurrentSite).first)) && (shift.second == (DistinctReps.at(CurrentRep).at(CurrentSite).second - ThisRep.at(CurrentSite).second)));
                                        CurrentSite++;
                                    }
            
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if ( !GlobalShifted )
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
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
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList.at(CurrentGraphHeight).at( gid ).Sites.size(); CurrentElement++)
                                {
                                    xMax = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first > xMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first : xMax;
                                    yMax = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second > yMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second : yMax;
                                    xMin = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first < xMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).first : xMin;
                                    yMin = (GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second < yMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Sites.at(CurrentElement).second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        std::vector< std::pair< int, int> > CheckList = DistinctReps.at(CurrentRep);
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps.at(CurrentRep).size(); CurrentSite++)
                                        {
                                            CheckList.at(CurrentSite).first += shift.first;
                                            CheckList.at(CurrentSite).second += shift.second;
                                            Counter += std::binary_search(GraphList.at(CurrentGraphHeight).at( gid ).Sites.begin(), GraphList.at(CurrentGraphHeight).at( gid ).Sites.end(), CheckList.at(CurrentSite));
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
                                GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back(make_pair(Embeddings, GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Identifier));
                            }
                        }
                    }   
                }
            }
        }
    }
}

void FindSubgraphs(vector< vector< BondGraph > > & GraphList)
{
    for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight < GraphList.size(); CurrentGraphHeight++) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.at(CurrentGraphHeight).size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                if( (unsigned int) gid < GraphList.at(CurrentGraphHeight).size() )
                {
                    GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back(std::make_pair(GraphList.at(CurrentGraphHeight).at( gid ).NumberSites(), 0));
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList.at(CurrentCheckHeight).size(); CurrentCheckWidth++ )
                        {
                            vector< vector< std::pair< std::pair<int,int>, std::pair<int,int> > > > DistinctReps;
                            DistinctReps.push_back( GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Bonds );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                                
                                Dihedral Transform(CurrentElement);
                                vector< std::pair< std::pair<int,int>, std::pair<int,int> > > ThisRep = GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Bonds;
                                for_each(ThisRep.begin(), ThisRep.end(), Transform);
                                for(unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                {
                                    std::pair<int,int> Temp = ThisRep.at(CurrentBond).first;
                                    if( ThisRep.at(CurrentBond).first > ThisRep.at(CurrentBond).second)
                                    {
                                        ThisRep.at(CurrentBond).first = ThisRep.at(CurrentBond).second;
                                        ThisRep.at(CurrentBond).second = Temp;
                                    }
                                }
                                std::sort(ThisRep.begin(), ThisRep.end());
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    const std::pair< int, int> shift1 = std::make_pair(DistinctReps.at(CurrentRep).front().first.first - ThisRep.front().first.first, DistinctReps.at(CurrentRep).front().first.second - ThisRep.front().first.second);
                                    const std::pair< int, int> shift2 = std::make_pair(DistinctReps.at(CurrentRep).front().second.first - ThisRep.front().second.first, DistinctReps.at(CurrentRep).front().second.second - ThisRep.front().second.second);
                                    bool Shifted = (shift1 == shift2);
                                    unsigned int CurrentBond = 1;
                                    while( Shifted && CurrentBond < DistinctReps.at(CurrentRep).size())
                                    {
                                        Shifted = Shifted && ((shift1.first == (DistinctReps.at(CurrentRep).at(CurrentBond).first.first - ThisRep.at(CurrentBond).first.first)) && (shift1.second == (DistinctReps.at(CurrentRep).at(CurrentBond).first.second - ThisRep.at(CurrentBond).first.second)));
                                        Shifted = Shifted && ((shift2.first == (DistinctReps.at(CurrentRep).at(CurrentBond).second.first - ThisRep.at(CurrentBond).second.first)) && (shift2.second == (DistinctReps.at(CurrentRep).at(CurrentBond).second.second - ThisRep.at(CurrentBond).second.second)));
                                        CurrentBond++;
                                    }
                    
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if (!GlobalShifted)
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first.first, -ThisRep.front().first.second);
                                    for( unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                    {
                                        ThisRep.at(CurrentBond).first.first += shift.first;
                                        ThisRep.at(CurrentBond).first.second += shift.second;
                                        ThisRep.at(CurrentBond).second.first += shift.first;
                                        ThisRep.at(CurrentBond).second.second += shift.second;
                                    }
                                    DistinctReps.push_back(ThisRep);
                                }
                            }

                            //Now we have all distinct permutations of the graph!

                            int Embeddings = 0;
                            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                            {
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList.at(CurrentGraphHeight).at( gid ).Bonds.size(); CurrentElement++)
                                {
                                    xMax = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.first > xMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.first : xMax;
                                    yMax = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.second > yMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.second : yMax;
                                    xMin = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.first < xMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.first : xMin;
                                    yMin = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.second < yMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        vector< std::pair< std::pair<int, int>, std::pair<int, int> > > CheckList = DistinctReps.at(CurrentRep);
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentBond = 0; CurrentBond < DistinctReps.at(CurrentRep).size(); CurrentBond++)
                                        {
                                            CheckList.at(CurrentBond).first.first += shift.first;
                                            CheckList.at(CurrentBond).first.second += shift.second;
                                            CheckList.at(CurrentBond).second.first += shift.first;
                                            CheckList.at(CurrentBond).second.second += shift.second;
                                            Counter += binary_search(GraphList.at(CurrentGraphHeight).at( gid ).Bonds.begin(), GraphList.at(CurrentGraphHeight).at( gid ).Bonds.end(), CheckList.at(CurrentBond));
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
                                GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back(std::make_pair(Embeddings, GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Identifier));
                            }
                        }
                    }
                }
            }
        }
    }
}

void FindSubgraphs(vector< vector< BondGraph > > & GraphList, unsigned int Index)
{
    unsigned int CurrentGraphHeight = Index - 1;
    if( CurrentGraphHeight < GraphList.size() ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.at(CurrentGraphHeight).size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                if( (unsigned int) gid < GraphList.at(CurrentGraphHeight).size() )
                {
                    GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back(std::make_pair(GraphList.at(CurrentGraphHeight).at( gid ).NumberSites(), 0));
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList.at(CurrentCheckHeight).size(); CurrentCheckWidth++ )
                        {
                            vector< vector< std::pair< std::pair<int,int>, std::pair<int,int> > > > DistinctReps;
                            DistinctReps.push_back( GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Bonds );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                                
                                Dihedral Transform(CurrentElement);
                                vector< std::pair< std::pair<int,int>, std::pair<int,int> > > ThisRep = GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Bonds;
                                for_each(ThisRep.begin(), ThisRep.end(), Transform);
                                for(unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                {
                                    std::pair<int,int> Temp = ThisRep.at(CurrentBond).first;
                                    if( ThisRep.at(CurrentBond).first > ThisRep.at(CurrentBond).second)
                                    {
                                        ThisRep.at(CurrentBond).first = ThisRep.at(CurrentBond).second;
                                        ThisRep.at(CurrentBond).second = Temp;
                                    }
                                }
                                std::sort(ThisRep.begin(), ThisRep.end());
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    const std::pair< int, int> shift1 = std::make_pair(DistinctReps.at(CurrentRep).front().first.first - ThisRep.front().first.first, DistinctReps.at(CurrentRep).front().first.second - ThisRep.front().first.second);
                                    const std::pair< int, int> shift2 = std::make_pair(DistinctReps.at(CurrentRep).front().second.first - ThisRep.front().second.first, DistinctReps.at(CurrentRep).front().second.second - ThisRep.front().second.second);
                                    bool Shifted = (shift1 == shift2);
                                    unsigned int CurrentBond = 1;
                                    while( Shifted && CurrentBond < DistinctReps.at(CurrentRep).size())
                                    {
                                        Shifted = Shifted && ((shift1.first == (DistinctReps.at(CurrentRep).at(CurrentBond).first.first - ThisRep.at(CurrentBond).first.first)) && (shift1.second == (DistinctReps.at(CurrentRep).at(CurrentBond).first.second - ThisRep.at(CurrentBond).first.second)));
                                        Shifted = Shifted && ((shift2.first == (DistinctReps.at(CurrentRep).at(CurrentBond).second.first - ThisRep.at(CurrentBond).second.first)) && (shift2.second == (DistinctReps.at(CurrentRep).at(CurrentBond).second.second - ThisRep.at(CurrentBond).second.second)));
                                        CurrentBond++;
                                    }
                    
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if (!GlobalShifted)
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first.first, -ThisRep.front().first.second);
                                    for( unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                    {
                                        ThisRep.at(CurrentBond).first.first += shift.first;
                                        ThisRep.at(CurrentBond).first.second += shift.second;
                                        ThisRep.at(CurrentBond).second.first += shift.first;
                                        ThisRep.at(CurrentBond).second.second += shift.second;
                                    }
                                    DistinctReps.push_back(ThisRep);
                                }
                            }

                            //Now we have all distinct permutations of the graph!

                            int Embeddings = 0;
                            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                            {
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList.at(CurrentGraphHeight).at( gid ).Bonds.size(); CurrentElement++)
                                {
                                    xMax = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.first > xMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.first : xMax;
                                    yMax = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.second > yMax) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).second.second : yMax;
                                    xMin = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.first < xMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.first : xMin;
                                    yMin = (GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.second < yMin) ? GraphList.at(CurrentGraphHeight).at( gid ).Bonds.at(CurrentElement).first.second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        vector< std::pair< std::pair<int, int>, std::pair<int, int> > > CheckList = DistinctReps.at(CurrentRep);
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentBond = 0; CurrentBond < DistinctReps.at(CurrentRep).size(); CurrentBond++)
                                        {
                                            CheckList.at(CurrentBond).first.first += shift.first;
                                            CheckList.at(CurrentBond).first.second += shift.second;
                                            CheckList.at(CurrentBond).second.first += shift.first;
                                            CheckList.at(CurrentBond).second.second += shift.second;
                                            Counter += binary_search(GraphList.at(CurrentGraphHeight).at( gid ).Bonds.begin(), GraphList.at(CurrentGraphHeight).at( gid ).Bonds.end(), CheckList.at(CurrentBond));
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
                                GraphList.at(CurrentGraphHeight).at( gid ).SubgraphList.push_back(std::make_pair(Embeddings, GraphList.at(CurrentCheckHeight).at(CurrentCheckWidth).Identifier));
                            }
                        }
                    }
                }
            }
        }
    }
}
void WriteGraphsToFile(std::vector<SiteGraph> & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++)
    {
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<GraphList.at(CurrentGraph).LowField<<endl;

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
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
        }
        Output<<endl;
    }
}

void WriteGraphsToFile(std::vector< std::vector<SiteGraph> > & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++)
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

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

void WriteGraphsToFile(std::vector< std::vector<SiteGraph> > & GraphList, string File, unsigned int Index)
{
    ofstream Output;
    Output.open(File.c_str(), std::ios::app);
    const unsigned int CurrentWidth = Index - 1;
    if( CurrentWidth < GraphList.size() )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;
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
    Output.close();
}

void WriteGraphsToFile(std::vector<BondGraph> & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++)
    {
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<GraphList.at(CurrentGraph).LowField<<endl;

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
    Output.close();
}

void WriteGraphsToFile(std::vector< std::vector<BondGraph> > & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++)
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

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
    Output.close();
}

void WriteGraphsToFile(std::vector< std::vector<BondGraph> > & GraphList, string File, unsigned int Index)
{
    ofstream Output;
    Output.open(File.c_str(), std::ios::app );
    const unsigned int CurrentWidth = Index - 1;
    if( CurrentWidth < GraphList.size() )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

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

void ReadGraphsFromFile( vector< Graph > & GraphList, const string & File)
{
  ifstream input(File.c_str());
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
