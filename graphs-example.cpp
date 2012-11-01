#include "graphs.h"

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
            const std::pair< int, int> shift = std::make_pair(SiteLists[ CurrentList ].front().first - TempSites.front().first, SiteLists[ CurrentList ].front().second - TempSites.front().second);
            
            unsigned int CurrentSite = 1;
            while( Shifted && CurrentSite < SiteLists[ CurrentList ].size())
            {
                int ShiftX = SiteLists[ CurrentList ][ CurrentSite ].first - TempSites[ CurrentSite ].first;
                int ShiftY = SiteLists[ CurrentList ][ CurrentSite ].second - TempSites[ CurrentSite ].second;
                Shifted = Shifted && (shift == std::make_pair(ShiftX, ShiftY);
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

bool SiteGraph::operator==(const SiteGraph & Other)
{
    if( ( Other.LatticeConstant == this->LatticeConstant) &&
        ( Other.Order == this->Order) )
    {
        for( int currentFactor = 0; currentFactor < 8; currentFactor++)
        {
            //! [Site isomorphism copy and transform]
            std::vector< std::pair< int, int> > SitesCopy = Other.Sites;
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
                int ShiftX = (this->Sites[ CurrentSite ].first - SitesCopy[ CurrentSite ].first);
                int ShiftY = (this->Sites[ CurrentSite ].second - SitesCopy[ CurrentSite ].second );
                Isomorphic = (Isomorphic) && (shift == std::make_pair(ShiftX, ShiftY));
                CurrentSite++;
            }
            if (this->AdjacencyList.size() > 0 && Other.AdjacencyList.size() > 0)
            {
                Isomorphic = Isomorphic || (this->AdjacencyList == Other.AdjacencyList);
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
            SitesCopy[ CurrentSite ] = std::make_pair(SitesCopy[ CurrentSite ].first + shift.first, SitesCopy[ CurrentSite ].second + shift.second);
        }
        
        if( lexicographical_compare(CanonicalSites.begin(), CanonicalSites.end(), SitesCopy.begin(), SitesCopy.end() ) )
        {
            CanonicalSites = SitesCopy;
        }
        //! [Shifting site canonical]
    }
    this->Sites = CanonicalSites;
}

bool BondGraph::operator==( const BondGraph & Other)
{
    if( (Other.Order == this->Order) )
    {
        for( int currentFactor = 0; currentFactor < 8; currentFactor++)
        {
            //! [Bond isomorphism copy and transform]
            std::vector< std::pair< std::pair< int, int>, std::pair< int, int> > > BondsCopy = Other.Bonds;
            Dihedral Transform(currentFactor);
            std::for_each(BondsCopy.begin(), BondsCopy.end(), Transform);
            for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
            {
                std::pair<int,int> Temp = BondsCopy.at(CurrentBond).first;
                if( BondsCopy[ CurrentBond ].first > BondsCopy[ CurrentBond ].second)
                {
                    BondsCopy[ CurrentBond ].first = BondsCopy[ CurrentBond ].second;
                    BondsCopy[ CurrentBond ].second = Temp;
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
                int ShiftX = this->Bonds[ CurrentBond ].first.first - BondsCopy[ CurrentBond ].first.first;
                int ShiftY = this->Bonds[ CurrentBond ].first.second - BondsCopy[ CurrentBond ].first.second;

                Isomorphic = Isomorphic && (shift1 == std::make_pair(ShiftX, ShiftY));
                
                ShiftX = this->Bonds[ CurrentBond ].second.first - BondsCopy[ CurrentBond ].second.first;
                ShiftY = this->Bonds[ CurrentBond ].second.second - BondsCopy[ CurrentBond ].second.second;

                Isomorphic = Isomorphic && (shift2 == std::make_pair(ShiftX, ShiftY));
                
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
            std::pair<int,int> Temp = BondsCopy[ CurrentBond ].first;
            if( BondsCopy[ CurrentBond ].first > BondsCopy[ CurrentBond ].second)
            {
                BondsCopy[ CurrentBond ].first = BondsCopy[ CurrentBond ].second;
                BondsCopy[ CurrentBond ].second = Temp;
            }
        }
        std::sort(BondsCopy.begin(), BondsCopy.end());
        //! [Bond canonical transform]
        //! [Bond canonical shift]
        std::pair<int,int> shift = std::make_pair(-BondsCopy.front().first.first, -BondsCopy.front().first.second);
        for(unsigned int CurrentBond = 0; CurrentBond < BondsCopy.size(); CurrentBond++)
        {
            BondsCopy[ CurrentBond ].first = std::make_pair( BondsCopy[ CurrentBond ].first.first + shift.first, BondsCopy[ CurrentBond ].first.second + shift.second);
            BondsCopy[ CurrentBond ].second = std::make_pair( BondsCopy[ CurrentBond ].second.first + shift.first, BondsCopy[ CurrentBond ].second.second + shift.second);
            LocalGraphKey += BondsCopy[ CurrentBond ].first.first*this->Order + BondsCopy[ CurrentBond ].first.second;
            LocalGraphKey += BondsCopy[ CurrentBond ].second.first*this->Order + BondsCopy[ CurrentBond ].second.second;
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
        SiteList.push_back( this->Bonds[ CurrentBond ].first );
        SiteList.push_back( this->Bonds[ CurrentBond ].second );
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
        FirstSite = std::find(SiteList.begin(), SiteList.end(), this->Bonds[ CurrentBond ].first);
        SecondSite = std::find(SiteList.begin(), SiteList.end(), this->Bonds[ CurrentBond ].second);
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
            std::pair<int,int> Temp = TempBonds[ CurrentBond ].first;
            if( TempBonds[ CurrentBond ].first > TempBonds[ CurrentBond ].second)
            {
                TempBonds[ CurrentBond ].first = TempBonds[ CurrentBond ].second;
                TempBonds[ CurrentBond ].second = Temp;
            }
        }
        std::sort(TempBonds.begin(), TempBonds.end());
        //! [Bond lattice constant transform]
        //! [Bond lattice constant shift check]
        bool GlobalShifted = false;
        for ( unsigned int CurrentList = 0; CurrentList < BondLists.size(); CurrentList++ )
        {
            const std::pair< int, int> shift1 = std::make_pair(BondLists[ CurrentList ].front().first.first - TempBonds.front().first.first, BondLists[ CurrentList ].front().first.second - TempBonds.front().first.second);
            const std::pair< int, int> shift2 = std::make_pair(BondLists[ CurrentList ].front().second.first - TempBonds.front().second.first, BondLists[ CurrentList ].front().second.second - TempBonds.front().second.second);
            bool Shifted = (shift1 == shift2); 
            unsigned int CurrentBond = 1;
            while( Shifted && CurrentBond < BondLists[ CurrentList ].size())
            {
                int ShiftX = BondLists[ CurrentList ][ CurrentBond ].first.first - TempBonds[ CurrentBond ].first.first;
                int ShiftY = BondLists[ CurrentList ][ CurrentBond ].first.second - TempBonds[ CurrentBond ].first.second;

                Shifted = Shifted && (shift1 == std::make_pair(ShiftX, ShiftY);
                
                ShiftX = BondLists[ CurrentList ][ CurrentBond ].second.first - TempBonds[ CurrentBond ].second.first;
                ShiftY = BondLists[ CurrentList ][ CurrentBond ].second.second - TempBonds[ CurrentBond ].second.second;
                
                Shifted = Shifted && (shift2 == std::make_pair(ShiftX, ShiftY));
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

void ConstructSiteBasedGraphs(std::vector< std::vector< SiteGraph > > & graphs, int FinalOrder)
{
    //! [Site grab back info]
    std::vector< SiteGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    //! [Site grab back info]
    while (CurrentOrder <= FinalOrder)
    {
        cout<<"Current Order: "<<CurrentOrder<<" Current Global ID: "<<GlobalIdentifier<<endl;   
        NewGraphs.clear();
        for( unsigned int CurrentGraph = 0; CurrentGraph < graphs.back().size(); CurrentGraph++)
        {
            int tid;
            //! [Site get graph ready]
            std::pair<int,int> NewSite;
            SiteGraph OldGraph = graphs.back().at(CurrentGraph);
            SiteGraph NewGraph;
            //! [Site get graph ready]
            omp_set_num_threads( CurrentOrder - 1 );
            #pragma omp parallel private(tid, NewSite, NewGraph) shared(OldGraph, NewGraphs, GlobalIdentifier)
            {   
                tid = omp_get_thread_num();
                //! [Site make new graph]
                for( int i = 0; i < 4; i++ )
                {
                    switch ( i )
                    {
                        case 0 :
                            NewSite = std::make_pair( OldGraph.Sites[ tid ].first + 1, OldGraph.Sites[ tid ].second);
                            break;
                        case 1 :
                            NewSite = std::make_pair( OldGraph.Sites[ tid ].first, OldGraph.Sites[ tid ].second + 1);
                            break;
                        case 2 :
                            NewSite = std::make_pair( OldGraph.Sites[ tid ].first - 1, OldGraph.Sites[ tid ].second);
                            break;
                        case 3 :
                            NewSite = std::make_pair( OldGraph.Sites[ tid ].first , OldGraph.Sites[ tid ].second - 1);
                            break;
                    }
            
                    if( !std::binary_search( OldGraph.Sites.begin(), OldGraph.Sites.end(), NewSite))
                    {
                        NewGraph = OldGraph;
                        NewGraph.AddSite( NewSite.first, NewSite.second);
                        NewGraph.Order = OldGraph.Order + 1;
                        NewGraph.MakeCanonical();
                        NewGraph.GenerateAdjacencyList();
                        
                        //! [Site make new graph]
                        bool Exists = false;
                        
                        #pragma omp critical
                        {
                            //! [Site check new graph]
                            for( unsigned int CurrentIndex = 0; CurrentIndex < NewGraphs.size(); CurrentIndex++ )
                            {
                                Exists = Exists || (NewGraph.Sites == NewGraphs[ CurrentIndex ].Sites );
                                //Exists = Exists || (NewGraph.AdjacencyList == NewGraphs.at(CurrentIndex).AdjacencyList); 
                            }
                            if( !Exists )
                            {
                                NewGraph.Identifier = ++GlobalIdentifier;
                                NewGraph.FindLatticeConstant();
                                NewGraph.LowField = false;
                                NewGraphs.push_back( NewGraph );
                            }
                            //! [Site check new graph]
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
    
    //! [First rectangles loops]

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalWidth; CurrentGraphWidth++)
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalHeight; CurrentGraphHeight++)
        {
            //! [First rectangles loops]
            //! [First rectangles temp storage]
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            std::vector< std::pair<int, int> > SiteList;
            SiteList.resize(CurrentOrder);
            std::vector< std::pair<int, int> > Subgraphs;
            //! [First rectangles temp storage]
            //! [First rectangles filling vectors]
            for( unsigned int CurrentCheckWidth = 1; CurrentCheckWidth <= CurrentGraphWidth; CurrentCheckWidth++)
            {
                for (unsigned int CurrentCheckHeight = 1; CurrentCheckHeight <= CurrentGraphHeight; CurrentCheckHeight++)
                {
                    SiteList.at(CurrentCheckHeight - 1 + (CurrentCheckWidth- 1)*CurrentGraphHeight) = std::make_pair(CurrentCheckWidth - 1, CurrentCheckHeight - 1);

                    int SubgraphCount = (CurrentGraphWidth + 1 - CurrentCheckWidth)*(CurrentGraphHeight + 1 - CurrentCheckHeight);
                    //! [First rectangles filling vectors]
                    //! [First rectangles check subgraph]
                    bool ThisGraph = (CurrentGraphHeight == CurrentCheckHeight) && (CurrentGraphWidth == CurrentCheckWidth);
                   
                    if( SubgraphCount > 0 &&
                        !ThisGraph
                        )
                    {
                        Subgraphs.push_back(std::make_pair( SubgraphCount, graphs.at(CurrentCheckWidth - 1).at(CurrentCheckHeight - 1).Identifier ) );
                    }
                    //! [First rectangles check subgraph]
                }
            }
            //! [First rectangles insert graph]
            SiteGraph NewGraph(SiteList, GlobalIdentifier++, CurrentOrder, 1, Subgraphs);
            NewGraph.GenerateAdjacencyList();
            NewGraph.LowField = false;
            graphs.at(CurrentGraphWidth - 1).push_back(NewGraph);
            //! [First rectangles insert graph]
        }
    }
}

void ConstructRectangularSiteGraphs(std::vector< std::vector< SiteGraph > > & graphs, unsigned int FinalOrder)
{
    graphs.resize(FinalOrder);
    int GlobalIdentifier = 0;
    
    //! [Second rectangles loops]

    for( unsigned int CurrentGraphWidth = 1; CurrentGraphWidth <= FinalOrder; CurrentGraphWidth++)
    {
        for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight <= FinalOrder && CurrentGraphHeight*CurrentGraphWidth <= FinalOrder; CurrentGraphHeight++)
        {
            unsigned int CurrentOrder = CurrentGraphHeight * CurrentGraphWidth;
            
            //! [Second rectangles loops]
            //! [Second rectangles temp storage]
            std::vector< std::pair<int, int> > SiteList;
            SiteList.resize(CurrentOrder);
            std::vector< std::pair<int, int> > Subgraphs;
            //! [Second rectangles temp storage]
            //! [Second rectangles filling vectors]
            for( unsigned int CurrentCheckWidth = 1; CurrentCheckWidth <= CurrentGraphWidth; CurrentCheckWidth++)
            {
                for (unsigned int CurrentCheckHeight = 1; CurrentCheckHeight <= CurrentGraphHeight; CurrentCheckHeight++)
                {
                    SiteList.at(CurrentCheckHeight - 1 + (CurrentCheckWidth- 1)*CurrentGraphHeight) = make_pair(CurrentCheckWidth - 1, CurrentCheckHeight - 1);

                    int SubgraphCount = (CurrentGraphWidth + 1 - CurrentCheckWidth)*(CurrentGraphHeight + 1 - CurrentCheckHeight);
                    //! [Second rectangles filling vectors]
                    //! [Second rectangles check subgraph]
                    bool ThisGraph = (CurrentGraphHeight == CurrentCheckHeight) && (CurrentGraphWidth == CurrentCheckWidth);
                   
                    if( SubgraphCount > 0 &&
                        !ThisGraph
                        )
                    {
                        Subgraphs.push_back(make_pair( SubgraphCount, graphs.at(CurrentCheckWidth - 1).at(CurrentCheckHeight - 1).Identifier ) );
                    }
                    //! [Second rectangles check subgraph]
                }
            }
            //! [Second rectangles insert graph]
            SiteGraph NewGraph(SiteList, GlobalIdentifier++, CurrentOrder, 1, Subgraphs);
            NewGraph.GenerateAdjacencyList();
            NewGraph.LowField = false;
            graphs.at(CurrentGraphWidth - 1).push_back(NewGraph);
            //! [Second rectangles insert graph]
        }
    }
}

void ConstructBondBasedGraphs(std::vector< std::vector< BondGraph > > & graphs, int FinalOrder)
{
    //! [Bond grab back info]
    std::vector< BondGraph > NewGraphs;
    int GlobalIdentifier = graphs.back().back().Identifier;
    int CurrentOrder = graphs.back().back().Order + 1;
    //! [Bond grab back info]
    while (CurrentOrder <= FinalOrder)
    {
        cout<<CurrentOrder<<endl;
        NewGraphs.clear();
        for( unsigned int CurrentGraph = 0; CurrentGraph < graphs.back().size(); CurrentGraph++)
        {
            //! [Bond get graph ready]
            BondGraph OldGraph = graphs.back().at(CurrentGraph);
            std::pair< std::pair< int, int>, std::pair< int, int> > NewBond;
            std::pair< int, int> NewSite;
            BondGraph NewGraph;
            //! [Bond get graph ready]
            int tid;
            #pragma omp parallel private(tid, NewSite, NewBond, NewGraph) shared(OldGraph, NewGraphs, GlobalIdentifier) num_threads(OldGraph.Bonds.size())
            {
                tid = omp_get_thread_num();
                //! [Bond make new graph]
                for( int i = 0; i < 6; i++)
                {
                        
                    switch( i )
                    {
                        case 0 :
                            if( OldGraph.Bonds[ tid ].first.first != OldGraph.Bonds[ tid ].second.first ) //Bond is horizontal
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].first.first, OldGraph.Bonds[ tid ].first.second + 1);
                                NewBond = std::make_pair(OldGraph.Bonds[ tid ].first, NewSite);
                            } //Add a bond on the left, pointing up
                            else 
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].second.first, OldGraph.Bonds[ tid ].second.second + 1);
                                NewBond = std::make_pair(OldGraph.Bonds[ tid ].second, NewSite);
                            } // Add a bond on the top, pointing up
                            break;
                        case 1 :
                        
                            if( OldGraph.Bonds[ tid ].first.first != OldGraph.Bonds[ tid ].second.first ) //Bond is horizontal
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].second.first, OldGraph.Bonds[ tid ].second.second + 1);
                                NewBond = std::make_pair(OldGraph.Bonds[ tid ].second, NewSite);
                            } //Add a bond on the right, pointing up
                            else 
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].second.first + 1, OldGraph.Bonds[ tid ].second.second);
                                NewBond = std::make_pair(OldGraph.Bonds[ tid ].second, NewSite);
                            } // Add a bond on top, pointing right
                            break;
                        case 2 :
                        
                            if( OldGraph.Bonds[ tid ].first.first != OldGraph.Bonds[ tid ].second.first ) //Bond is horizontal
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].second.first + 1, OldGraph.Bonds[ tid ].second.second);
                                NewBond = std::make_pair(OldGraph.Bonds[ tid ].second, NewSite);
                            } // Add a bond on the right, pointing right
                            else 
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].first.first + 1, OldGraph.Bonds[ tid ].first.second);
                                NewBond = std::make_pair(OldGraph.Bonds[ tid ].first, NewSite);
                            } // Add a bond on the bottom, pointing right
                            break;
                        case 3 : 
                        
                            if( OldGraph.Bonds[ tid ].first.first != OldGraph.Bonds[ tid ].second.first ) //Bond is horizontal
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].first.first - 1, OldGraph.Bonds[ tid ].first.second);
                                NewBond = std::make_pair(NewSite, OldGraph.Bonds[ tid ].first);
                            } // Add a bond on the left, pointing left
                            else 
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].second.first - 1, OldGraph.Bonds[ tid ].second.second);
                                NewBond = std::make_pair(NewSite, OldGraph.Bonds[ tid ].second);
                            } // Add a bond on the top, pointing left
                            break;
                        case 4 : 
                        
                            if( OldGraph.Bonds[ tid ].first.first != OldGraph.Bonds[ tid ].second.first ) //Bond is horizontal
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].first.first, OldGraph.Bonds[ tid ].first.second - 1);
                                NewBond = std::make_pair(NewSite, OldGraph.Bonds[ tid ].first);
                            } // Add a bond to the left, pointing down
                            else 
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].first.first - 1, OldGraph.Bonds[ tid ].first.second);
                                NewBond = std::make_pair(NewSite, OldGraph.Bonds[ tid ].first);
                            } // Add a bond to the bottom, pointing left
                            break;
                        case 5 : 
                        
                            if( OldGraph.Bonds[ tid ].first.first != OldGraph.Bonds[ tid ].second.first ) //Bond is horizontal
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].second.first, OldGraph.Bonds[ tid ].second.second - 1);
                                NewBond = std::make_pair(NewSite, OldGraph.Bonds[ tid ].second);
                            } // Add a bond to the right, pointing down
                            else 
                            {
                                NewSite = std::make_pair(OldGraph.Bonds[ tid ].first.first, OldGraph.Bonds[ tid ].first.second - 1);
                                NewBond = std::make_pair(NewSite, OldGraph.Bonds[ tid ].first);
                            } // Add a bond to the bottom, pointing down
                            break;

                    }
                
                    if( !std::binary_search( OldGraph.Bonds.begin(), OldGraph.Bonds.end(), NewBond))
                    {
                        NewGraph = OldGraph;
                        NewGraph.AddBond( NewBond.first, NewBond.second);
                        NewGraph.Order = OldGraph.Order + 1;
                        NewGraph.MakeCanonical();
                        //! [Bond make new graph]
                        //! [Bond check new graph]
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
                        //! [Bond check new graph]
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
    //! [Single vector site find subgraphs search]
    for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList.size(); CurrentGraphGroup++)
    {
        #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup) num_threads( MaxSize )
        {
            tid = omp_get_thread_num();
            gid = tid + CurrentGraphGroup * MaxSize;
            //! [Single vector site find subgraphs search]
            if ( (unsigned int) gid < GraphList.size() )
            {
                //! [Single vector site find subgraphs single] 
                GraphList[ gid ].SubgraphList.push_back(std::make_pair(GraphList[ gid ].Order, 0));
                //! [Single vector site find subgraphs single] 
    
                unsigned int CurrentCheck = 1;
                //! [Single vector site find subgraphs reps] 
                while (CurrentCheck < (unsigned int) gid && GraphList.at(CurrentCheck).Order < GraphList[ gid ].Order)
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
                            const std::pair< int, int> shift = std::make_pair(DistinctReps[ CurrentRep ].front().first - ThisRep.front().first, DistinctReps[ CurrentRep ].front().second - ThisRep.front().second);
            
                            unsigned int CurrentSite = 1;
                            while( Shifted && CurrentSite < DistinctReps[ CurrentRep ].size())
                            {
                                Shifted = Shifted && ((shift.first == (DistinctReps[ CurrentRep ][ CurrentSite ].first - ThisRep[ CurrentSite ].first)) && (shift.second == (DistinctReps[ CurrentRep ][ CurrentSite ].second - ThisRep[ CurrentSite ].second)));
                                CurrentSite++;
                            }
                    
                            GlobalShifted = GlobalShifted || Shifted;
                        }
                        if (!GlobalShifted)
                        {
                            const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
                            for( unsigned int CurrentSite = 0; CurrentSite < ThisRep.size(); CurrentSite++)
                            {
                                ThisRep[ CurrentSite ].first += shift.first;
                                ThisRep[ CurrentSite ].second += shift.second;
                            }
                            DistinctReps.push_back(ThisRep);
                        }
                    }

                    //Now we have all distinct permutations of the graph!
                    //! [Single vector site find subgraphs reps] 
                    //! [Single vector site find subgraphs shift count]

                    int Embeddings = 0;
                    for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                    {
                        std::pair< int, int> shift = std::make_pair(0,0);
                        int xMax = 0;
                        int yMax = 0;
                        int xMin = 0;
                        int yMin = 0;
                        for( unsigned int CurrentElement = 0; CurrentElement < GraphList[ gid ].Sites.size(); CurrentElement++)
                        {
                            xMax = (GraphList[ gid ].Sites[ CurrentElement ].first > xMax) ? GraphList[ gid ].Sites[ CurrentElement ].first : xMax;
                            yMax = (GraphList[ gid ].Sites[ CurrentElement ].second > yMax) ? GraphList[ gid ].Sites[ CurrentElement ].second : yMax;
                            xMin = (GraphList[ gid ].Sites[ CurrentElement ].first < xMin) ? GraphList[ gid ].Sites[ CurrentElement ].first : xMin;
                            yMin = (GraphList[ gid ].Sites[ CurrentElement ].second < yMin) ? GraphList[ gid ].Sites[ CurrentElement ].second : yMin;
                        }

                        for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                        {
                            for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                            {
                                shift = std::make_pair(xBoost, yBoost);
                                std::vector< std::pair< int, int> > CheckList = DistinctReps[ CurrentRep ];
                                unsigned int Counter = 0;
                                for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps[ CurrentRep ].size(); CurrentSite++)
                                {
                                    CheckList[ CurrentSite ].first += shift.first;
                                    CheckList[ CurrentSite ].second += shift.second;
                                    Counter += std::binary_search(GraphList[ gid ].Sites.begin(), GraphList[ gid ].Sites.end(), CheckList[ CurrentSite ]);
                                }
                                if ( Counter == CheckList.size() )
                                {
                                            Embeddings++;
                                }    
                            }
                            if (Embeddings > 0)
                            {
                                GraphList[ gid ].SubgraphList.push_back(make_pair(Embeddings, GraphList.at(CurrentCheck).Identifier));
                            }
                        }
                    }
                    //! [Single vector site find subgraphs shift count]
                    CurrentCheck++;
                }
            }
        }
    }
}

void FindSubgraphs(std::vector< std::vector< SiteGraph > > & GraphList)
{
    //! [Double vector site find subgraphs search]
    for( unsigned int CurrentGraphHeight = 1; CurrentGraphHeight < GraphList.size(); CurrentGraphHeight++) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {

                    //! [Double vector site find subgraphs search]
                    //! [Double vector site find subgraphs single]
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(GraphList[ CurrentGraphHeight ][ gid ].Order, 0) );
                    //! [Double vector site find subgraphs single]
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {
                            //! [Double vector site find subgraphs reps]
                            std::vector< std::vector< std::pair< int, int> > > DistinctReps;
                            DistinctReps.push_back( GraphList[ CurrentCheckHeight ].at( CurrentCheckWidth ).Sites );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                         
                                Dihedral Transform(CurrentElement);
                                std::vector< std::pair< int, int> > ThisRep = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Sites;
                                std::for_each( ThisRep.begin(), ThisRep.end(), Transform );
                                std::sort( ThisRep.begin(), ThisRep.end() );
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    bool Shifted = true;
                                    const std::pair< int, int> shift = make_pair(DistinctReps[ CurrentRep ].front().first - ThisRep.front().first, DistinctReps[ CurrentRep ].front().second - ThisRep.front().second);
            
                                    unsigned int CurrentSite = 1;
                                    while( Shifted && CurrentSite < DistinctReps[ CurrentRep ].size())
                                    {
                                        Shifted = Shifted && ((shift.first == (DistinctReps[ CurrentRep ][ CurrentSite ].first - ThisRep[ CurrentSite ].first)) && (shift.second == (DistinctReps[ CurrentRep ][ CurrentSite ].second - ThisRep[ CurrentSite ].second)));
                                        CurrentSite++;
                                    }
            
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if ( !GlobalShifted )
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
                                    for( unsigned int CurrentSite = 0; CurrentSite < ThisRep.size(); CurrentSite++)
                                    {
                                        ThisRep[ CurrentSite ].first += shift.first;
                                        ThisRep[ CurrentSite ].second += shift.second;
                                    }
                                    DistinctReps.push_back(ThisRep);
                                }
                            }
                            //! [Double vector site find subgraphs reps]

                            //Now we have all distinct permutations of the graph!

                            //! [Double vector site find subgraphs shift count]
                            int Embeddings = 0;
                            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                            {
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList[ CurrentGraphHeight ][ gid ].Sites.size(); CurrentElement++)
                                {
                                    xMax = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first > xMax) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first : xMax;
                                    yMax = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second > yMax) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second : yMax;
                                    xMin = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first < xMin) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first : xMin;
                                    yMin = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second < yMin) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        std::vector< std::pair< int, int> > CheckList = DistinctReps[ CurrentRep ];
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps[ CurrentRep ].size(); CurrentSite++)
                                        {
                                            CheckList[ CurrentSite ].first += shift.first;
                                            CheckList[ CurrentSite ].second += shift.second;
                                            Counter += std::binary_search(GraphList[ CurrentGraphHeight ][ gid ].Sites.begin(), GraphList[ CurrentGraphHeight ][ gid ].Sites.end(), CheckList[ CurrentSite ]);
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
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(Embeddings, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier));
                            }
                            //! [Double vector site find subgraphs shift count]
                        }
                    }   
                }
            }
        }
    }
}

void FindSubgraphs(std::vector< std::vector< SiteGraph > > & GraphList, unsigned int Index)
{
    //! [Double vector site index find subgraphs search]
    unsigned int CurrentGraphHeight = Index - 1;
    if ( CurrentGraphHeight < GraphList.size() ) 
    {
        int tid;
        int gid;
        int MaxSize = omp_get_num_procs();
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                //! [Double vector site index find subgraphs search]
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {

                    //! [Double vector site index find subgraphs single]
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(GraphList[ CurrentGraphHeight ][ gid ].Order, 0) );
                    //! [Double vector site index find subgraphs single]
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {
                            //! [Double vector site index find subgraphs reps]
                            std::vector< std::vector< std::pair< int, int> > > DistinctReps;
                            DistinctReps.push_back( GraphList[ CurrentCheckHeight ].at( CurrentCheckWidth ).Sites );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                         
                                Dihedral Transform(CurrentElement);
                                std::vector< std::pair< int, int> > ThisRep = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Sites;
                                std::for_each( ThisRep.begin(), ThisRep.end(), Transform );
                                std::sort( ThisRep.begin(), ThisRep.end() );
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    bool Shifted = true;
                                    const std::pair< int, int> shift = make_pair(DistinctReps[ CurrentRep ].front().first - ThisRep.front().first, DistinctReps[ CurrentRep ].front().second - ThisRep.front().second);
            
                                    unsigned int CurrentSite = 1;
                                    while( Shifted && CurrentSite < DistinctReps[ CurrentRep ].size())
                                    {
                                        Shifted = Shifted && ((shift.first == (DistinctReps[ CurrentRep ][ CurrentSite ].first - ThisRep[ CurrentSite ].first)) && (shift.second == (DistinctReps[ CurrentRep ][ CurrentSite ].second - ThisRep[ CurrentSite ].second)));
                                        CurrentSite++;
                                    }
            
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if ( !GlobalShifted )
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first, -ThisRep.front().second);
                                    for( unsigned int CurrentSite = 0; CurrentSite < ThisRep.size(); CurrentSite++)
                                    {
                                        ThisRep[ CurrentSite ].first += shift.first;
                                        ThisRep[ CurrentSite ].second += shift.second;
                                    }
                                    DistinctReps.push_back(ThisRep);
                                }
                            }
                            //! [Double vector site index find subgraphs reps]

                            //Now we have all distinct permutations of the graph!
                            //! [Double vector site index find subgraphs shift count]

                            int Embeddings = 0;
                            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                            {
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList[ CurrentGraphHeight ][ gid ].Sites.size(); CurrentElement++)
                                {
                                    xMax = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first > xMax) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first : xMax;
                                    yMax = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second > yMax) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second : yMax;
                                    xMin = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first < xMin) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].first : xMin;
                                    yMin = (GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second < yMin) ? GraphList[ CurrentGraphHeight ][ gid ].Sites[ CurrentElement ].second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        std::vector< std::pair< int, int> > CheckList = DistinctReps[ CurrentRep ];
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentSite = 0; CurrentSite < DistinctReps[ CurrentRep ].size(); CurrentSite++)
                                        {
                                            CheckList[ CurrentSite ].first += shift.first;
                                            CheckList[ CurrentSite ].second += shift.second;
                                            Counter += std::binary_search(GraphList[ CurrentGraphHeight ][ gid ].Sites.begin(), GraphList[ CurrentGraphHeight ][ gid ].Sites.end(), CheckList[ CurrentSite ]);
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
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back( std::make_pair(Embeddings, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier));
                            }
                            //! [Double vector site index find subgraphs shift count]
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
        //! [Double vector bond find subgraphs search]
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                //! [Double vector bond find subgraphs search]
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {
                    //! [Double vector bond find subgraphs single]
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back(std::make_pair(GraphList[ CurrentGraphHeight ][ gid ].NumberSites(), 0));
                    //! [Double vector bond find subgraphs single]
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {
                            //! [Double vector bond find subgraphs reps]
                            vector< vector< std::pair< std::pair<int,int>, std::pair<int,int> > > > DistinctReps;
                            DistinctReps.push_back( GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Bonds );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                                
                                Dihedral Transform(CurrentElement);
                                vector< std::pair< std::pair<int,int>, std::pair<int,int> > > ThisRep = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Bonds;
                                for_each(ThisRep.begin(), ThisRep.end(), Transform);
                                for(unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                {
                                    std::pair<int,int> Temp = ThisRep[ CurrentBond ].first;
                                    if( ThisRep[ CurrentBond ].first > ThisRep[ CurrentBond ].second)
                                    {
                                        ThisRep[ CurrentBond ].first = ThisRep[ CurrentBond ].second;
                                        ThisRep[ CurrentBond ].second = Temp;
                                    }
                                }
                                std::sort(ThisRep.begin(), ThisRep.end());
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    const std::pair< int, int> shift1 = std::make_pair(DistinctReps[ CurrentRep ].front().first.first - ThisRep.front().first.first, DistinctReps[ CurrentRep ].front().first.second - ThisRep.front().first.second);
                                    const std::pair< int, int> shift2 = std::make_pair(DistinctReps[ CurrentRep ].front().second.first - ThisRep.front().second.first, DistinctReps[ CurrentRep ].front().second.second - ThisRep.front().second.second);
                                    bool Shifted = (shift1 == shift2);
                                    unsigned int CurrentBond = 1;
                                    while( Shifted && CurrentBond < DistinctReps[ CurrentRep ].size())
                                    {
                                        Shifted = Shifted && ((shift1.first == (DistinctReps[ CurrentRep ][ CurrentBond ].first.first - ThisRep[ CurrentBond ].first.first)) && (shift1.second == (DistinctReps[ CurrentRep ][ CurrentBond ].first.second - ThisRep[ CurrentBond ].first.second)));
                                        Shifted = Shifted && ((shift2.first == (DistinctReps[ CurrentRep ][ CurrentBond ].second.first - ThisRep[ CurrentBond ].second.first)) && (shift2.second == (DistinctReps[ CurrentRep ][ CurrentBond ].second.second - ThisRep[ CurrentBond ].second.second)));
                                        CurrentBond++;
                                    }
                    
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if (!GlobalShifted)
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first.first, -ThisRep.front().first.second);
                                    for( unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                    {
                                        ThisRep[ CurrentBond ].first.first += shift.first;
                                        ThisRep[ CurrentBond ].first.second += shift.second;
                                        ThisRep[ CurrentBond ].second.first += shift.first;
                                        ThisRep[ CurrentBond ].second.second += shift.second;
                                    }
                                    DistinctReps.push_back(ThisRep);
                                }
                            }

                            //Now we have all distinct permutations of the graph!
                            //! [Double vector bond find subgraphs reps]
                            //! [Double vector bond find subgraphs shift count]
                            int Embeddings = 0;
                            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                            {
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList[ CurrentGraphHeight ][ gid ].Bonds.size(); CurrentElement++)
                                {
                                    xMax = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.first > xMax) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.first : xMax;
                                    yMax = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.second > yMax) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.second : yMax;
                                    xMin = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.first < xMin) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.first : xMin;
                                    yMin = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.second < yMin) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        vector< std::pair< std::pair<int, int>, std::pair<int, int> > > CheckList = DistinctReps[ CurrentRep ];
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentBond = 0; CurrentBond < DistinctReps[ CurrentRep ].size(); CurrentBond++)
                                        {
                                            CheckList[ CurrentBond ].first.first += shift.first;
                                            CheckList[ CurrentBond ].first.second += shift.second;
                                            CheckList[ CurrentBond ].second.first += shift.first;
                                            CheckList[ CurrentBond ].second.second += shift.second;
                                            Counter += binary_search(GraphList[ CurrentGraphHeight ][ gid ].Bonds.begin(), GraphList[ CurrentGraphHeight ][ gid ].Bonds.end(), CheckList[ CurrentBond ]);
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
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back(std::make_pair(Embeddings, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier));
                            }
                            //! [Double vector bond find subgraphs shift count]
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
        //! [Double vector bond index find subgraphs search]
        for( unsigned int CurrentGraphGroup = 0; CurrentGraphGroup*MaxSize <= GraphList[ CurrentGraphHeight ].size(); CurrentGraphGroup++)
        {
            omp_set_num_threads( MaxSize );
            #pragma omp parallel private(tid, gid) shared(MaxSize, CurrentGraphGroup, CurrentGraphHeight)
            {
                tid = omp_get_thread_num();
                gid = tid + (CurrentGraphGroup * MaxSize);
                //! [Double vector bond index find subgraphs search]
                if( (unsigned int) gid < GraphList[ CurrentGraphHeight ].size() )
                {
                
                    //! [Double vector bond index find subgraphs single]
                    GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back(std::make_pair(GraphList[ CurrentGraphHeight ][ gid ].NumberSites(), 0));
                    //! [Double vector bond index find subgraphs single]
                
                    for( unsigned int CurrentCheckHeight = 1; CurrentCheckHeight < CurrentGraphHeight; CurrentCheckHeight++ )
                    {
                    
                        for(unsigned int CurrentCheckWidth = 0; CurrentCheckWidth < GraphList[ CurrentCheckHeight ].size(); CurrentCheckWidth++ )
                        {
                            //! [Double vector bond index find subgraphs reps]
                            vector< vector< std::pair< std::pair<int,int>, std::pair<int,int> > > > DistinctReps;
                            DistinctReps.push_back( GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Bonds );

                            for( int CurrentElement = 1; CurrentElement < 8; CurrentElement++)
                            {
                                
                                Dihedral Transform(CurrentElement);
                                vector< std::pair< std::pair<int,int>, std::pair<int,int> > > ThisRep = GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Bonds;
                                for_each(ThisRep.begin(), ThisRep.end(), Transform);
                                for(unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                {
                                    std::pair<int,int> Temp = ThisRep[ CurrentBond ].first;
                                    if( ThisRep[ CurrentBond ].first > ThisRep[ CurrentBond ].second)
                                    {
                                        ThisRep[ CurrentBond ].first = ThisRep[ CurrentBond ].second;
                                        ThisRep[ CurrentBond ].second = Temp;
                                    }
                                }
                                std::sort(ThisRep.begin(), ThisRep.end());
                                bool GlobalShifted = false;
                                for ( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++ )
                                {
                                    const std::pair< int, int> shift1 = std::make_pair(DistinctReps[ CurrentRep ].front().first.first - ThisRep.front().first.first, DistinctReps[ CurrentRep ].front().first.second - ThisRep.front().first.second);
                                    const std::pair< int, int> shift2 = std::make_pair(DistinctReps[ CurrentRep ].front().second.first - ThisRep.front().second.first, DistinctReps[ CurrentRep ].front().second.second - ThisRep.front().second.second);
                                    bool Shifted = (shift1 == shift2);
                                    unsigned int CurrentBond = 1;
                                    while( Shifted && CurrentBond < DistinctReps[ CurrentRep ].size())
                                    {
                                        Shifted = Shifted && ((shift1.first == (DistinctReps[ CurrentRep ][ CurrentBond ].first.first - ThisRep[ CurrentBond ].first.first)) && (shift1.second == (DistinctReps[ CurrentRep ][ CurrentBond ].first.second - ThisRep[ CurrentBond ].first.second)));
                                        Shifted = Shifted && ((shift2.first == (DistinctReps[ CurrentRep ][ CurrentBond ].second.first - ThisRep[ CurrentBond ].second.first)) && (shift2.second == (DistinctReps[ CurrentRep ][ CurrentBond ].second.second - ThisRep[ CurrentBond ].second.second)));
                                        CurrentBond++;
                                    }
                    
                                    GlobalShifted = GlobalShifted || Shifted;
                                }
                                if (!GlobalShifted)
                                {
                                    const std::pair< int, int> shift = std::make_pair(-ThisRep.front().first.first, -ThisRep.front().first.second);
                                    for( unsigned int CurrentBond = 0; CurrentBond < ThisRep.size(); CurrentBond++)
                                    {
                                        ThisRep[ CurrentBond ].first.first += shift.first;
                                        ThisRep[ CurrentBond ].first.second += shift.second;
                                        ThisRep[ CurrentBond ].second.first += shift.first;
                                        ThisRep[ CurrentBond ].second.second += shift.second;
                                    }
                                    DistinctReps.push_back(ThisRep);
                                }
                            }

                            //! [Double vector bond index find subgraphs reps]
                            //Now we have all distinct permutations of the graph!

                            //! [Double vector bond index find subgraphs shift count]
                            int Embeddings = 0;
                            for( unsigned int CurrentRep = 0; CurrentRep < DistinctReps.size(); CurrentRep++)
                            {
                                std::pair< int, int> shift = std::make_pair(0,0);
                                int xMax = 0;
                                int xMin = 0; 
                                int yMax = 0;
                                int yMin = 0;
                                for( unsigned int CurrentElement = 0; CurrentElement < GraphList[ CurrentGraphHeight ][ gid ].Bonds.size(); CurrentElement++)
                                {
                                    xMax = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.first > xMax) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.first : xMax;
                                    yMax = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.second > yMax) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].second.second : yMax;
                                    xMin = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.first < xMin) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.first : xMin;
                                    yMin = (GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.second < yMin) ? GraphList[ CurrentGraphHeight ][ gid ].Bonds[ CurrentElement ].first.second : yMin;
                                }
                                for( int xBoost = xMin; xBoost <= xMax; xBoost++)
                                {
                                    for( int yBoost = yMin; yBoost <= yMax; yBoost++)
                                    {
                                        shift = std::make_pair(xBoost, yBoost);
                                        vector< std::pair< std::pair<int, int>, std::pair<int, int> > > CheckList = DistinctReps[ CurrentRep ];
                                        unsigned int Counter = 0;
                                        for( unsigned int CurrentBond = 0; CurrentBond < DistinctReps[ CurrentRep ].size(); CurrentBond++)
                                        {
                                            CheckList[ CurrentBond ].first.first += shift.first;
                                            CheckList[ CurrentBond ].first.second += shift.second;
                                            CheckList[ CurrentBond ].second.first += shift.first;
                                            CheckList[ CurrentBond ].second.second += shift.second;
                                            Counter += binary_search(GraphList[ CurrentGraphHeight ][ gid ].Bonds.begin(), GraphList[ CurrentGraphHeight ][ gid ].Bonds.end(), CheckList[ CurrentBond ]);
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
                                GraphList[ CurrentGraphHeight ][ gid ].SubgraphList.push_back(std::make_pair(Embeddings, GraphList[ CurrentCheckHeight ][ CurrentCheckWidth ].Identifier));
                            }
                            //! [Double vector bond index find subgraphs shift count]
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
        //! [Single vector site first]
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<GraphList.at(CurrentGraph).LowField<<endl;

        //! [Single vector site first]
        //! [Single vector site second]
        for (unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentGraph).Sites.size(); CurrentSite++)
        {
            Output<<GraphList.at(CurrentGraph).Sites.at(CurrentSite).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).Sites.at(CurrentSite).second;
            Output<<" ";
        }
        Output<<endl;
        //! [Single vector site second]
        //! [Single vector site third]
        for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentGraph).AdjacencyList.size(); CurrentConnection++)
        {
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).second;
            Output<<" ";
        }
        Output<<endl;
        //! [Single vector site third]
        //! [Single vector site fourth]
        for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentGraph).SubgraphList.size(); CurrentSubgraph++)
        {
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
        }
        Output<<endl;
        //! [Single vector site fourth]
    }
}

void WriteGraphsToFile(std::vector< std::vector<SiteGraph> > & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentWidth = 0; CurrentWidth < GraphList.size(); CurrentWidth++)
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {
            //! [Double vector site first]

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

            //! [Double vector site first]
            //! [Double vector site second]
            for (unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentWidth).at(CurrentHeight).Sites.size(); CurrentSite++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).second;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector site second]
            //! [Double vector site third]
            for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector site third]
            //! [Double vector site fourth]
            for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector site fourth]
        }
    }
}

void WriteGraphsToFile(std::vector< std::vector<SiteGraph> > & GraphList, string File, unsigned int Index)
{
    //! [Site append]
    ofstream Output;
    Output.open(File.c_str(), std::ios::app);
    //! [Site append]
    const unsigned int CurrentWidth = Index - 1;
    if( CurrentWidth < GraphList.size() )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {
            //! [Double vector index site first]

            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;
            //! [Double vector index site first]
            //! [Double vector index site second]
            for (unsigned int CurrentSite = 0; CurrentSite < GraphList.at(CurrentWidth).at(CurrentHeight).Sites.size(); CurrentSite++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Sites.at(CurrentSite).second;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector index site second]
            //! [Double vector index site third]
            for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector index site third]
            //! [Double vector index site fourth]
            for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector index site fourth]
        }
    }
    Output.close();
}

void WriteGraphsToFile(std::vector<BondGraph> & GraphList, string File)
{
    ofstream Output(File.c_str());
    for( unsigned int CurrentGraph = 0; CurrentGraph < GraphList.size(); CurrentGraph++)
    {
        //! [Single vector bond first]
        Output<<GraphList.at(CurrentGraph).Identifier<<" ";
        Output<<GraphList.at(CurrentGraph).Order<<" ";
        Output<<GraphList.at(CurrentGraph).LatticeConstant<<" ";
        Output<<GraphList.at(CurrentGraph).LowField<<endl;
        //! [Single vector bond first]
        //! [Single vector bond second]

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
        //! [Single vector bond second]
        //! [Single vector bond third]
        for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentGraph).AdjacencyList.size(); CurrentConnection++)
        {
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).AdjacencyList.at(CurrentConnection).second;
            Output<<" ";
        }
        Output<<endl;
        //! [Single vector bond third]
        //! [Single vector bond fourth]
        for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentGraph).SubgraphList.size(); CurrentSubgraph++)
        {
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).first;
            Output<<" ";
            Output<<GraphList.at(CurrentGraph).SubgraphList.at(CurrentSubgraph).second;
            Output<<" ";
        }
        Output<<endl;
        //! [Single vector bond fourth]
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

            //! [Double vector bond first]
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

            //! [Double vector bond first]
            //! [Double vector bond second]
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
            //! [Double vector bond second]
            //! [Double vector bond third]
            for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector bond third]
            //! [Double vector bond fourth]
            for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
            } 
            Output<<endl;
            //! [Double vector bond fourth]
        }
    }
    Output.close();
}

void WriteGraphsToFile(std::vector< std::vector<BondGraph> > & GraphList, string File, unsigned int Index)
{
    //! [Bond append]
    ofstream Output;
    Output.open(File.c_str(), std::ios::app );
    //! [Bond append]
    const unsigned int CurrentWidth = Index - 1;
    if( CurrentWidth < GraphList.size() )
    {
        for( unsigned int CurrentHeight = 0; CurrentHeight < GraphList.at(CurrentWidth).size(); CurrentHeight++)
        {

            //! [Double vector index bond first]
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Identifier<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).Order<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LatticeConstant<<" ";
            Output<<GraphList.at(CurrentWidth).at(CurrentHeight).LowField<<endl;

            //! [Double vector index bond first]
            //! [Double vector index bond second]
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
            //! [Double vector index bond second]
            //! [Double vector index bond third]
            for (unsigned int CurrentConnection = 0; CurrentConnection < GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.size(); CurrentConnection++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).AdjacencyList.at(CurrentConnection).second;
                Output<<" ";
            }
            Output<<endl;
            //! [Double vector index bond third]
            //! [Double vector index bond fourth]
            for (unsigned int CurrentSubgraph = 0; CurrentSubgraph < GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.size(); CurrentSubgraph++)
            {
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).first;
                Output<<" ";
                Output<<GraphList.at(CurrentWidth).at(CurrentHeight).SubgraphList.at(CurrentSubgraph).second;
                Output<<" ";
            } 
            Output<<endl;
            //! [Double vector index bond fourth]
        }
    }
    Output.close();
}

void ReadGraphsFromFile( vector< Graph > & GraphList, const string & File)
{
    //! [Read in file]    
    ifstream input(File.c_str());
    vector< string > rawLines;
    Graph tempGraph;
    
    while ( !input.eof() )
    {
        rawLines.resize(rawLines.size() + 1);
        getline(input, rawLines.back()) ; 
    }

    input.close();

    stringstream ss (stringstream::in | stringstream::out);

    //! [Read in file]

    for  (unsigned int currentLine = 0; currentLine < rawLines.size()-1; currentLine+=4)
    {

        //! [Fill containers]
        string currentNumber;
    
        ss << rawLines.at(currentLine);
        
        ss >> tempGraph.Identifier;
        ss >> tempGraph.Order;
        ss >> tempGraph.LatticeConstant;
        ss >> tempGraph.LowField;
        
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
        }

        ss.str("");
        ss.clear();
        //! [Fill containers]
        //! [Finish graph]
        GraphList.push_back(tempGraph);
        //! [Finish graph]
    }   
}
