#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <iomanip>
#include "WeightedGraph.hpp"

using namespace std;


int main(){

    WeightedGraph* graph;
    ifstream infile("test06.in");
    string line;
    while (getline(infile, line)) {
        if (line == "exit") {
            delete graph;
            return 0;
        }
        else if (line == "edge_count") {
            std::cout << "edge count is " << graph->numEdges << "\n";
        }
        else if (line == "clear") {
            graph->clearEdges();
        }
        else {
            long unsigned int separatorIndex = line.find(" ");
            //Separate command (first word) from arguements (everything else)
            string command = line.substr(0, separatorIndex);
            string arguementWord = line.substr(separatorIndex + 1, line.length() - separatorIndex - 1);
            if (command == "n") {
                graph = new WeightedGraph(std::stoi(arguementWord));
                std::cout << "success\n";
            }
            else if (command == "degree") {
                graph->getNodeDegree(std::stoi(arguementWord));
            }
            else if (command == "mst") {
                graph->mst(std::stoi(arguementWord));
            }
            else
            {
                separatorIndex = arguementWord.find(";");
                //Separate command (first word) from arguements (everything else)
                string arg1 = arguementWord.substr(0, separatorIndex);
                string arg2 = arguementWord.substr(separatorIndex + 1, line.length() - separatorIndex - 1);
                if (command == "e") {
                    graph->eraseEdge(std::stoi(arg1), std::stoi(arg2));
                }
                else {
                    separatorIndex = arg2.find(";");
                    string arg3 = arg2.substr(separatorIndex + 1, line.length() - separatorIndex - 1);
                    arg2 = arg2.substr(0, separatorIndex);

                    if (command == "i") {
                        graph->addEdge(std::stoi(arg1), std::stoi(arg2), std::stod(arg3));
                    }
                    else if (command == "adjacent") {
                        graph->checkAdjacent(std::stoi(arg1), std::stoi(arg2), std::stod(arg3));
                    }
                }
            }
        }
    }
    delete graph;
    return 0;
}