#pragma once
class PQnode {
public:
	int vertex; //origin
	double key; //weight

	PQnode(int v, double k) {
		vertex = v;
		key = k;
	}
	~PQnode() {
		vertex = 0;
		key = 0;
	}
};
