#include "graphs.h"


int main()
{
    int maxOrder = 20;
    vector< graph > linearGraphs;
    int currentIndex = 0;
    for( int currentOrder = 1; currentOrder <= maxOrder; currentOrder++)
    {
        currentIndex++;
        linearGraphs.resize(currentIndex);
        linearGraphs[currentIndex - 1].NumberSites = currentOrder;
        linearGraphs[currentIndex - 1].NumberBonds = currentOrder - 1;
        linearGraphs[currentIndex - 1].AdjacencyList.resize(currentOrder - 1);
        if (currentOrder > 1)
        {
            for ( int currentSite = 0; currentSite < currentOrder - 1; currentSite++)
            {
            
                linearGraphs.at(currentIndex - 1).AdjacencyList.at(currentSite) = make_pair(currentSite, currentSite + 1);
            }
        }
        linearGraphs[currentIndex - 1].Identifier = currentOrder - 1;
        for ( int currentGraph = 0; currentGraph < currentOrder; currentGraph++)
        {
            linearGraphs[currentIndex - 1].SubgraphList.push_back(currentGraph);
        }
    }

    const std::string filename ("lineargraphs.dat");
    WriteGraphsToFile( linearGraphs, filename);
}

void WriteGraphsToFile( vector< graph > & graphList, const std::string file)
{
    ofstream output(file.c_str());
    for( unsigned int currentGraph = 0; currentGraph < graphList.size(); currentGraph++)
    {
        output<<graphList[currentGraph].Identifier<<endl;
        output<<graphList[currentGraph].NumberSites<<endl;
        output<<graphList[currentGraph].NumberBonds<<endl;
        output<<graphList[currentGraph].LatticeConstant<<endl;

        for (unsigned int currentBond = 0; currentBond < graphList[currentGraph].AdjacencyList.size(); currentBond++)
        {
            output<<"("<<graphList[currentGraph].AdjacencyList[currentBond].first<<","<<graphList[currentGraph].AdjacencyList[currentBond].second<<")";
        }
        output<<endl;

        for (unsigned int currentSubgraph = 0; currentSubgraph < graphList[currentGraph].SubgraphList.size(); currentSubgraph++)
        {
            output<<"("<<graphList[currentGraph].SubgraphList[currentSubgraph]<<")";
        }
        output<<endl;
    }
}

void ReadGraphsFromFile( vector< graph > & graphList, string & file)
{
    ifstream input(file.c_str());
    vector< string > rawLines;
    int currentGraph;
    const int memberCount = 6;

    while ( !input.eof() )
    {
        rawLines.resize(rawLines.size() + 1);
        getline(input, rawLines.back()) ; 
    }

    input.close();

    for (unsigned int currentLine = 0; currentLine < rawLines.size(); currentLine++)
    {
        currentGraph = currentLine/memberCount;
        graph tempGraph;
        unsigned int currentChar = 0;
        string currentNumber;
        switch ( currentLine % memberCount )
        {
            case 0 :
                tempGraph.Identifier = atoi(rawLines.at(currentLine).c_str());
            case 1 :
                tempGraph.NumberSites = atoi(rawLines.at(currentLine).c_str());
                break;
            case 2 :
                tempGraph.NumberBonds = atoi(rawLines.at(currentLine).c_str());
                break;
            case 3 : 
                tempGraph.LatticeConstant = atoi(rawLines.at(currentLine).c_str());
                break;
            case 4 : 
                
                while ( currentChar < rawLines.at(currentLine).length() )
                {
                    if ( rawLines.at(currentLine)[currentChar] == '(' )
                    {
                        tempGraph.AdjacencyList.resize( tempGraph.AdjacencyList.size() + 1);
                    }
                    if ( rawLines.at(currentLine)[currentChar] != '(' &&
                         rawLines.at(currentLine)[currentChar] != ')' &&
                         rawLines.at(currentLine)[currentChar] != ',' && 
                         rawLines.at(currentLine)[currentChar] != '\n' )
                    {
                        currentNumber.push_back(rawLines.at(currentLine)[currentChar]);
                    }
                    if ( rawLines.at(currentLine)[currentChar] == ',' )
                    {
                        tempGraph.AdjacencyList.back().first = atoi(currentNumber.c_str());
                        currentNumber.clear();
                    }
                    if ( rawLines.at(currentLine)[currentChar] == ')' )
                    {
                        tempGraph.AdjacencyList.back().second = atoi(currentNumber.c_str());
                        currentNumber.clear();
                    }
                }
                break;
            case 5 :

                while ( currentChar < rawLines.at(currentLine).length() )
                {
                    if ( rawLines.at(currentLine)[currentChar] == '(' )
                    {
                        tempGraph.SubgraphList.resize( tempGraph.SubgraphList.size() + 1);
                    }
                    
                    if ( rawLines.at(currentLine)[currentChar] != '(' &&
                         rawLines.at(currentLine)[currentChar] != ')' &&
                         rawLines.at(currentLine)[currentChar] != '\n' )
                    {
                        currentNumber.push_back(rawLines.at(currentLine)[currentChar]);
                    }
                    if ( rawLines.at(currentLine)[currentChar] == ')' )
                    {
                        tempGraph.SubgraphList.back() = atoi(currentNumber.c_str());
                        currentNumber.clear();
                    }
                }
                break;
        }
            
        graphList.push_back(tempGraph);
        
    }
}

/*
std::string getFileContents(const std::string& filename)
{
        std::ifstream file(filename.c_str());
        return std::string(std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>());
}
*/
