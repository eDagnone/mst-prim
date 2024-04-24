#pragma once

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "edge.hpp"
#include "PQ_min.hpp"
#include "PQnode.hpp"

using namespace std;
class illegal_exception : public exception {
	const char* what() const throw() {
		return "illegal_exception";
	}
} illegal_exception;

struct LinkedList {
	Edge* head; // points to head of linked list of edges.
};

class WeightedGraph
{
public:
	int numNodes;
	int numEdges;
	LinkedList* edges;

	void addEdge(int, int, double);
	void eraseEdge(int, int);
	Edge* checkAdjacent(int, int);
	void checkAdjacent(int, int, double);
	void clearEdges();
	int getNodeDegree(int);
	void mst(int);
	void printNodes();
	WeightedGraph(){
		numNodes = 0;
		numEdges = 0;
		edges = nullptr;
	}
	WeightedGraph(int graphSize) {

		numNodes = graphSize;
		numEdges = 0;
		numNodes = graphSize;

		// Create an array of adjacency lists.  Size of array will be V
		edges = new LinkedList[graphSize];

		// Initialize each adjacency list as empty by making head as NULL
		for (int i = 0; i < graphSize; ++i){
			edges[i].head = nullptr;
		}
	}
	~WeightedGraph() {
		for (int i = 0; i < numNodes; i++) {
			while (edges[i].head != nullptr) {
				Edge* thisNode = edges[i].head;
				edges[i].head = thisNode->next;
				delete thisNode;
				//I don't care about setting the parents of the next node to be nullptr, 
				//because the parents are getting deleted anyways. 
			}
		}
		delete[] edges;
	}
};

void WeightedGraph::addEdge(int node1, int node2, double weight) {
	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 || node1 == node2 || weight <= 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal arguement\n";
		}
	}
	else {
		Edge* existingEdge = checkAdjacent(node1, node2);
		if (existingEdge != nullptr) {
			existingEdge->weight = weight;
			existingEdge = checkAdjacent(node2, node1);
			existingEdge->weight = weight;
		}
		else{
			numEdges += 2;
			// Add an edge from node 1 to 2 and 2 to the front of each adjacency list
			Edge* newEdge = new Edge(node2, weight, edges[node1].head);
			edges[node1].head = newEdge;

			// Edge from node 2 to 1
			newEdge = new Edge(node1, weight, edges[node2].head);
			edges[node2].head = newEdge;
		}
		std::cout << "success\n";
	}
	return;
}
void WeightedGraph::eraseEdge(int node1, int node2) {
	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 ) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal arguement\n";
		}
	}
	else {
		Edge* edge = checkAdjacent(node1, node2);
		if (edge != nullptr) {
			if (edge->parent != nullptr) {
				edge->parent->next = edge->next;
			}
			else {
				edges[node1].head = edge->next;
			}
			if (edge->next != nullptr)		edge->next->parent = edge->parent;
			delete edge;
		}
		edge = checkAdjacent(node2, node1);
		if (edge != nullptr) {
			if (edge->parent != nullptr) {
				edge->parent->next = edge->next;
			}
			else {
				edges[node2].head = edge->next;
			}
			if (edge->next != nullptr)		edge->next->parent = edge->parent;
			delete edge;
		}
		numEdges -= 2;
		std::cout << "success (erase)" << node1 << " " << node2 <<"\n";
	}	
	return;
}


void WeightedGraph::checkAdjacent(int node1, int node2, double weight) {

	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 || node1 == node2 || weight <= 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal arguement\n";
		}
	}
	else {
		Edge* edge = checkAdjacent(node1, node2);
		if (edge != nullptr) {
			if (edge->weight == weight) {
				std::cout << "adjacent " << node1 << " " << node2 << " " << std::fixed << std::setprecision(2) << weight << std::endl;
				return;
			}
		}
		std::cout << "not adjacent " << node1 << " " << node2 << " " << std::fixed << std::setprecision(2)<< weight << std::endl;
	}
	return;
}
Edge* WeightedGraph::checkAdjacent(int node1, int node2) {

	if (node1 >= numNodes || node1 < 0 || node2 >= numNodes || node2 < 0 || node1 == node2 ) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal arguement\n";
		}
	}
	else {
		Edge* currentEdge = edges[node1].head;
		while(currentEdge != nullptr) {
			if (currentEdge->destination == node2) {
				return currentEdge;
			}
			currentEdge = currentEdge->next;
		}
	}
	return nullptr;
}

void WeightedGraph::clearEdges() {
	for (int i = 0; i < numNodes; i++) {
		while (edges[i].head != nullptr) {
			Edge* thisNode = edges[i].head;
			edges[i].head = thisNode->next;
			delete thisNode;
			//We don't care about setting the parents of the next node to be nullptr, 
			//because the parents are getting deleted anyways. 
		}		
	}
	std::cout << "success\n";
	numEdges = 0;
}


//I only had time to make this algorithm run in O(ElgV) time, or make it start at node u. 
//I chose the former. It's the same number regardless of where you start anyways.
void WeightedGraph::mst(int rootNode) {
	if (rootNode >= numNodes || rootNode < 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal arguement\n";
		}
	}
	else {
		for (int i = 0; i < numNodes; i++) {
			if (edges[i].head == nullptr) {
				std::cout << "not connected\n";
				return;
			}
		}
		printNodes();

		double key[numNodes];
		key[0] = 0;
		
		PQ_min* priorityQueue = new PQ_min(numNodes);
		priorityQueue->queueNodes[0] = new PQnode(0, 0); //First item in queue
		priorityQueue->position[0] = 0;

		for (int i = 1; i < numNodes; ++i) {
			key[i] = INT_MAX;
			priorityQueue->queueNodes[i] = new PQnode(i, key[i]);
			priorityQueue->position[i] = i;
		}
		

		priorityQueue->size = numNodes; //Queue is now full (of inaccurate entries)		

		// Continuously get lowest edge
		while (priorityQueue->size != 0) {
			PQnode* PQ_minNode = priorityQueue->getMin();
			//Rectify key values as all PQnodes are traversed
			Edge* currentEdge = edges[PQ_minNode->vertex].head;
			while (currentEdge != nullptr) {
				int vertex = currentEdge->destination;
				if (priorityQueue->hasVertex(vertex) && currentEdge->weight < key[vertex]) {
					key[vertex] = currentEdge->weight;
					priorityQueue->decreaseKey(vertex, key[vertex]);
				}
				currentEdge = currentEdge->next;
			}
		}
		//Keys are all correct - add them, print, and return
		double total = 0;
		for (int i = 1; i < numNodes; i++){
			total += key[i];
		}
		std::cout << "mst " << std::fixed << std::setprecision(2) << total<< std::endl;
		//delete priorityQueue;
	}
}
int WeightedGraph::getNodeDegree(int node) {
	int degree = 0;
	if (node >= numNodes || node < 0) {
		try {
			throw illegal_exception;
		}
		catch (exception& e) {
			std::cout << "illegal arguement\n";
		}
	}
	else {
		Edge* currentEdge = edges[node].head;
		while (currentEdge != nullptr) {
			degree++;
			currentEdge = currentEdge->next;
		}
		std::cout << "Degree of " << node << " is " << degree << std::endl;
	}
	return degree;
}

void WeightedGraph::printNodes() {
	for (int i = 0; i < numNodes; i++) {
		Edge* currentNode = edges[i].head;
		while (currentNode != nullptr) {
			std::cout << "Edge from " << i << " to " << currentNode->destination << " is " << currentNode->weight << std::endl;
			currentNode = currentNode->next;
		}
	}
}








