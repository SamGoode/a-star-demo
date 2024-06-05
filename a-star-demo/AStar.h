#pragma once
#include "int2.h"
#include "Array.h"
#include "Node.h"
#include "NavGrid.h"

class AStar {
private:
	NavGrid* grid;

	Node* startNode;
	Node* currentNode;

	Array<Node*> container;

	Array<Node*> openNodes;
	Array<Node*> closedNodes;

public:
	bool pathFound;
	bool running;

public:
	AStar(NavGrid* _grid);
	~AStar();

	const Array<Node*>& getOpenNodes() { return openNodes; }
	const Array<Node*>& getClosedNodes() { return closedNodes; }
	Node* getCurrentNode() { return currentNode; }

	float getDist(int2 pos1, int2 pos2);

	Array<int2> getAdjPositions(int2 pos);

	int searchOpenNodes(int2 pos);
	int searchClosedNodes(int2 pos);
	
	void restart();
	void setup();

	void update();
	void draw();
	void showInfo(int x, int y);
};