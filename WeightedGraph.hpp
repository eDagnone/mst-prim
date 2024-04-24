#pragma once

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

using namespace std;
class illegal_exception : public exception {
	const char* what() const throw() {
		return "illegal_exception";
	}
} illegal_exception;


class WeightedGraph
{
public:
	int numEdges;

	void addEdge(int, int, double);
	void eraseEdge(int, int);
	void checkAdjacent(int, int, double);
	void clearEdges();
	void getNodeDegree(int);
	void mst(int);
	WeightedGraph(){
		numNodes = 0;
		numEdges = 0;
		weights = nullptr;
		nodeDegrees = nullptr;
	}
	WeightedGraph(int graphSize) {

		numNodes = graphSize;
		numEdges = 0;
		numNodes = graphSize;

		// Create weight matrix, initialize all values to 0.
		weights = new double* [graphSize];
		nodeDegrees = new int[graphSize];
		for (int i = 0; i < graphSize; ++i) {
			nodeDegrees[i] = 0;
			weights[i] = new double[graphSize];
			for (int k = 0; k < graphSize; ++k) {
				weights[i][k] = 0;
			}
		}
		
	}
	~WeightedGraph() {
		for (int i = 0; i < numNodes; i++) {
			delete[] weights[i];
			
		}
		delete[] weights;
		delete[] nodeDegrees;
	}
private:
	int numNodes;
	int* nodeDegrees;
	double** weights;
};

void WeightedGraph::addEdge(int node1, int node2, double weight) {
	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 || node1 == node2 || weight <= 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal argument\n";
		}
	}
	else {
		if (weights[node1][node2] == 0) {
			nodeDegrees[node1] ++;
			nodeDegrees[node2] ++;
			numEdges += 2;
		}
		weights[node1][node2] = weight;
		weights[node2][node1] = weight;
		std::cout << "success\n";
	}
	return;
}
void WeightedGraph::eraseEdge(int node1, int node2) {
	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 || node1==node2 ) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal argument\n";
		}
	}
	else {
		if (weights[node1][node2] == 0) { //Edge does not exist
			std::cout << "failure\n";
		}
		else {
			numEdges -= 2;
			weights[node1][node2] = 0;
			weights[node2][node1] = 0;
			nodeDegrees[node1] --;
			nodeDegrees[node2] --;
			std::cout << "success\n";
		}
	}	
	return;
}


void WeightedGraph::checkAdjacent(int node1, int node2, double weight) {

	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 || node1 == node2 || weight <= 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal argument\n";
		}
	}
	else {
		if(weights[node1][node2] == weight){
				std::cout << "adjacent " << node1 << " " << node2 << " " << std::fixed << std::setprecision(2) << weight << std::endl;
		}
		else {
			std::cout << "not adjacent " << node1 << " " << node2 << " " << std::fixed << std::setprecision(2) << weight << std::endl;
		}
	}
	return;
}


void WeightedGraph::clearEdges() {
	for (int i = 0; i < numNodes; ++i) {
		for (int k = 0; k < numNodes; ++k) {
			weights[i][k] = 0;
		}
		nodeDegrees[i] = 0;
	}
	std::cout << "success\n";
	numEdges = 0;
}



void WeightedGraph::mst(int rootNode) {
	if (rootNode >= numNodes || rootNode < 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal argument\n";
		}
	}
	else{
		double key[numNodes];
		bool inMST[numNodes];
		for (int i = 0; i < numNodes; i++) {
			key[i] = DBL_MAX;
			inMST[i] = false;
		}
		key[rootNode] = 0;
		for (int i = 0; i < numNodes - 1; i++)
		{
			// Add smallest vertex to MST
			double min = DBL_MAX;
			int min_index;
			for (int vertex = 0; vertex < numNodes; vertex++) {
				if ( key[vertex] < min && !inMST[vertex]) {
					min_index = vertex;
					min = key[vertex];
				}
			}
			int vertex1 = min_index; //Rename for clarity
			inMST[vertex1] = true;
			for (int vertex2 = 0; vertex2 < numNodes; vertex2++) {
				if (weights[vertex1][vertex2] && weights[vertex1][vertex2] < key[vertex2] && !inMST[vertex2]) {
					key[vertex2] = weights[vertex1][vertex2];
				}
			}
		}
		double total = 0;
		for (int i = 0; i < numNodes; i++) {
			total += key[i];
		}
		if (total >= DBL_MAX || total < 0)
			std::cout << "not connected\n";
		else
			std::cout << "mst " << std::fixed << std::setprecision(2) << total<< std::endl;
	}
}
void WeightedGraph::getNodeDegree(int node) {
	if (node >= numNodes || node < 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal argument\n";
		}
	}
	else {
		std::cout << "degree of " << node << " is " << nodeDegrees[node] << std::endl;
	}
	return;
}