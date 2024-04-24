#include "PQnode.hpp"
#pragma once

class PQ_min {
public:
	int capacity;
	int size; // Current # of actual nodes (edges)
	int* position; // For decreaseKey()
	PQnode** queueNodes;

	void heapifyMin(int);
	void decreaseKey(int, double);
	PQnode* getMin();
	void swapNodes(PQnode**, PQnode**);
	bool hasVertex(int);
	PQ_min(int capacity) {
		this->capacity = capacity;
		size = 0;
		//queueNodes = new PQnode * [capacity];
		//position = new int[capacity];
		position = (int*)malloc(capacity * sizeof(int));
		queueNodes = (PQnode**)malloc(capacity * sizeof(PQnode*));
	}
	/*~PQ_min() {
		delete[] position;
		for (int i = 0; i < capacity; i++) {
			//std::cout<<queueNodes[i]<<std::endl;
			//delete[] queueNodes[i];
		}
		delete[] queueNodes;
	}*/
};

/*PROBABLY HERE*/
void PQ_min::decreaseKey(int vrtx, double key)
{
	// Find vertex in heap array
	int index = position[vrtx];
	int parentIndex = (index - 1) / 2;
	std::cout << "Index " <<queueNodes[index]->key << "\t" << queueNodes[index]->vertex << std::endl;
	std::cout << "Parent "<<queueNodes[parentIndex]->key << "\t" << queueNodes[parentIndex]->vertex << std::endl;

	queueNodes[index]->key = key;

	// Heapify by ascending through tree
	while (queueNodes[parentIndex]->key > queueNodes[index]->key && index) {
		// Swap node and parent
		position[queueNodes[index]->vertex] = parentIndex;
		position[queueNodes[parentIndex]->vertex] = index;
		swapNodes(&queueNodes[index], &queueNodes[parentIndex]);

		index = parentIndex;
	}
	
}

// Destroy the head node, then get the min.
PQnode* PQ_min::getMin()
{
	if (size < 0)	return nullptr;

	PQnode* root = queueNodes[0];

	// Swap root node and last node
	PQnode* lastNode = queueNodes[size - 1];
	queueNodes[0] = lastNode;
	position[root->vertex] = size - 1;
	position[lastNode->vertex] = 0;

	--size; //gives the illusion of destroying the last node. 
	//Re-heapify to get large lastNode out of the root.
	heapifyMin(0);

	std::cout << root->key << "\t" << root->vertex << std::endl;
	return root;
}

//MAY HAPPEN HERE
// Sort array into min heap. Runs based on size variable.
void PQ_min::heapifyMin(int parentNode)
{
	int smallest = parentNode;
	int brLeft = 2 * parentNode + 1; //Left branch index
	int brRight = 2 * parentNode + 2; //Right branch indes

	//Check if child is smaller. If so, prepare to swap.
	if (brLeft < size) {
		if (queueNodes[brLeft]->key < queueNodes[smallest]->key)
			smallest = brLeft;
	}
	if (brRight < size) {
		if (queueNodes[brRight]->key < queueNodes[smallest]->key)
			smallest = brRight;
	}

	//Swap if child smaller
	if (smallest != parentNode) {
		PQnode* smallestNode = queueNodes[smallest];
		PQnode* parent = queueNodes[parentNode];

		position[smallestNode->vertex] = parentNode;
		position[parent->vertex] = smallest;
		swapNodes(&queueNodes[parentNode], &queueNodes[smallest]);
		heapifyMin(smallest);
	}
}


void PQ_min::swapNodes(PQnode** a, PQnode** b)
{
	PQnode* temp = *a;
	*a = *b;
	*b = temp;
}

bool PQ_min::hasVertex(int vrtx) {
	return size > position[vrtx];
}