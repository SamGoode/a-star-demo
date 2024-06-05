#pragma once
#include "int2.h"

struct Node {
public:
	int2 pos;
	float gCost;
	float hCost;
	Node* parent;

public:
	float fCost() {
		return gCost + hCost;
	}
};

