#include "graphs.h"

void WriteGraphsToFile( vector< graph > & graphList, string & file)
{
    ofstream output.open(file);
    for( unsigned int currentGraph = 0; currentGraph < graphList.size(); currentGraph++)
    {
        output<<currentGraph.NumberSites<<endl;
        output<<currentGraph.NumberBonds<<endl;
        output<<currentGraph.LatticeConstant<<endl;

        for (unsigned int currentBond = 0; currentBond < graphList[currentGraph].AdjacencyList.size(); currentBond++)
        {
            output<<"("<<graphList[currentGraph].AdjacencyList[currentBond].first<<","<<graphList[currentGraph].AdjacencyList[currentBond].second<<")";
        }
        output<<endl;

        for (unsigned int currentSubgraph = 0; currentSubgraph < graphList[currentGraph].SubgraphList.size(); currentSubgraph++)
        {
            output<<"("<<graphList[currentGraph].SubgraphList[currentSubgraph]<<")"<<endl;
        }
    }
}

void ReadGraphsFromFile( vector< graph > & graphList, string & file)
{
    ifstream input.open(file);
    vector< string > rawLines;
    int currentGraph;
    const int memberCount = 5;

    while ( !input.eof() )
    {
        rawLines.push_back( getline(input) ); 
    }

    input.close();

    for (unsigned int currentLine = 0; currentLine < rawLines.size(); currentLine++)
    {
        currentGraph = currentLine/memberCount;
        graph tempGraph;
        switch ( currentLine % memberCount )
        {
            case 0 :
                tempGraph.NumberSites = atoi(rawLines.at(currentLine));
                break;
            case 1 :
                tempGraph.NumberBonds = atoi(rawLines.at(currentLine));
                break;
            case 2 : 
                tempGraph.LatticeConstant = atoi(rawLines.at(currentLine));
                break;
            case 3 : 
                unsigned int currentChar = 0;
                string currentNumber;
                
                while ( currentChar < rawLines.at(currentLine).length() )
                {
                    if ( rawLines.at(currentLine)[currentChar] == "(" )
                    {
                        tempGraph.AdjacencyList.push_back();
                    }
                    if ( rawLines.at(currentLine)[currentChar] != "(" &&
                         rawLines.at(currentLine)[currentChar] != ")" &&
                         rawLines.at(currentLine)[currentChar] != "," && 
                         rawLines.at(currentLine)[currentChar] != "\n" )
                    {
                        currentNumber.push_back(rawLines.at(currentLine)[currentChar]);
                    }
                    if ( rawLines.at(currentLine)[currentChar] == "," )
                    {
                        tempGraph.AdjacencyList.end().first = atoi(currentNumber);
                        currentNumber.clear();
                    }
                    if ( rawLines.at(currentLine)[currentChar] == ")" )
                    {
                        tempGraph.AdjacencyList.end().second = atoi(currentNumber);
                        currentNumber.clear();
                    }
                }
                break;
            case 4 :
                unsigned int currentChar = 0;
                string currentNumber;

                while ( currentChar < rawLines.at(currentLine).length() )
                {
                    if ( rawLines.at(currentLine)[currentChar] == "(" )
                    {
                        tempGraph.SubgraphList.push_back();
                    }
                    
                    if ( rawLines.at(currentLine)[currentChar] != "(" &&
                         rawLines.at(currentLine)[currentChar] != ")" &&
                         rawLines.at(currentLine)[currentChar] != "\n" )
                    {
                        currentNumber.push_back(rawLines.at(currentLine)[currentChar]);
                    }
                    if ( rawLines.at(currentLine)[currentChar] == ")" )
                    {
                        tempGraph.SubgraphList.end() = atoi(currentNumber);
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
