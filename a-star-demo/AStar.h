#pragma once
#include "int2.h"
#include "Array.h"
#include "Node.h"
#include "NavGrid.h"

class AStar {
private:
	NavGrid* grid;

	int2 startPos;
	int2 destPos;

	Node* startNode;
	Node* currentNode;

	Array<Node*> container;

	Array<Node*> openNodes;
	Array<Node*> closedNodes;

public:
	bool pathFound;
	bool noPathFound;

public:
	AStar(NavGrid* _grid, int2 _startPos, int2 _destPos);
	~AStar();

	const Array<Node*>& getOpenNodes() { return openNodes; }
	const Array<Node*>& getClosedNodes() { return closedNodes; }
	Node* getCurrentNode() { return currentNode; }

	bool validPos(int2 pos);

	float getDist(int2 pos1, int2 pos2);

	Array<int2> getAdjPositions(int2 pos);

	void restart();

	int searchOpenNodes(int2 pos);
	int searchClosedNodes(int2 pos);

	void update();
	void draw();
	void showInfo(int x, int y);
};