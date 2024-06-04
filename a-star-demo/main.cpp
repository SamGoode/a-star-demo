#include <string>
#include <algorithm>
#include "raylib.h"
#include "NavGrid.h"
#include "Array.h"

struct Node {
public:
	int2 pos;
	int gCost;
	int hCost;
	Node* parent;

public:
	//Node() {
	//	pos = { 0, 0 };
	//	gCost = 0;
	//	hCost = 0;
	//	parent = nullptr;
	//}

	//Node(int2 _pos, int _gCost, int _hCost, Node* _parent) {
	//	pos = _pos;
	//	gCost = _gCost;
	//	hCost = _hCost;
	//	parent = _parent;
	//}

	//Node(const Node& copy) {
	//	pos = copy.pos;
	//	gCost = copy.gCost;
	//	hCost = copy.hCost;
	//	parent = copy.parent;
	//}

	//Node& operator=(const Node& copy) {
	//	pos = copy.pos;
	//	gCost = copy.gCost;
	//	hCost = copy.hCost;
	//	parent = copy.parent;

	//	return *this;
	//}

	int getFCost() {
		return gCost + hCost;
	}
};

bool compareGCost(Node* a, Node* b) {
	return a->gCost < b->gCost;
}

bool compareFCost(Node* a, Node* b) {
	return a->getFCost() < b->getFCost();
}

class AStar {
private:
	int2 startPos;
	int2 destPos;

	Node* startNode;
	Node* currentNode;

	Array<Node*> container;

	Array<Node*> openNodes;
	Array<Node*> closedNodes;

public:
	bool pathFound;
	
public:
	AStar(int2 _startPos, int2 _destPos) {
		startPos = _startPos;
		destPos = _destPos;

		startNode = new Node { startPos, 0, getHCost(startPos), nullptr };
		currentNode = nullptr;

		container = Array<Node*>(0, 16);

		openNodes = Array<Node*>(0, 16);
		closedNodes = Array<Node*>(0, 16);

		pathFound = false;

		container.append(startNode);
		openNodes.append(startNode);
	}

	~AStar() {
		for (int i = 0; i < container.getCount(); i++) {
			delete container[i];
		}
	}

	const Array<Node*>& getOpenNodes() {
		return openNodes;
	}

	const Array<Node*>& getClosedNodes() {
		return closedNodes;
	}

	Node* getCurrentNode() {
		return currentNode;
	}

	bool validPos(int2 pos) {
		return pos.x >= 0 && pos.x < 20 && pos.y >= 0 && pos.y < 20;
	}

	int getHCost(int2 pos) {
		return abs(pos.x - destPos.x) + abs(pos.y - destPos.y);
	}

	Array<int2> getAdjPositions(int2 pos) {
		Array<int2> adjPositions(0, 4);

		int2 offsets[4] = {
			{0, -1}, {1, 0}, {0, 1}, {-1, 0}
		};

		for (int i = 0; i < 4; i++) {
			int2 adjPos = pos + offsets[i];
			if (validPos(adjPos)) {
				adjPositions.append(adjPos);
			}
		}

		return adjPositions;
	}

	void restart() {
		for (int i = 0; i < container.getCount(); i++) {
			delete container[i];
		}

		startNode = new Node { startPos, 0, getHCost(startPos), nullptr};
		currentNode = nullptr;
		
		container = Array<Node*>(0, 16);

		openNodes = Array<Node*>(0, 16);
		closedNodes = Array<Node*>(0, 16);

		pathFound = false;

		container.append(startNode);
		openNodes.append(startNode);
	}

	int searchOpenNodes(int2 pos) {
		for (int i = 0; i < openNodes.getCount(); i++) {
			if (openNodes[i]->pos == pos) {
				return i;
			}
		}

		return -1;
	}

	int searchClosedNodes(int2 pos) {
		for (int i = 0; i < closedNodes.getCount(); i++) {
			if (closedNodes[i]->pos == pos) {
				return i;
			}
		}

		return -1;
	}

	void update() {
		if (pathFound) {
			return;
		}

		if (openNodes.getCount() == 0) {
			throw "no viable path found";
		}

		//if (openNodes.getCount() > 1) {
		std::sort(openNodes.begin(), openNodes.end(), compareFCost);
		//}

		currentNode = openNodes[0];

		if (currentNode->pos == destPos) {
			pathFound = true;
			return;
		}

		openNodes.remove(0);
		closedNodes.append(currentNode);

		Array<int2> adjPositions = getAdjPositions(currentNode->pos);
		
		for (int i = 0; i < adjPositions.getCount(); i++) {
			if (searchClosedNodes(adjPositions[i]) != -1) {
				continue;
			}

			int gCost = currentNode->gCost + 1;
			int hCost = getHCost(adjPositions[i]);
			int fCost = gCost + hCost;

			int index = searchOpenNodes(adjPositions[i]);

			if (index == -1) {
				container.append(new Node { adjPositions[i], gCost, hCost, currentNode });
				openNodes.append(container[container.getCount() - 1]);
			}
			else if(fCost < openNodes[index]->getFCost()) {
				//int index2 = searchNodes(adjPositions[i]);
				//nodes[index2] = { adjPositions[i], gCost, hCost, currentNode };
				*openNodes[index] = { adjPositions[i], gCost, hCost, currentNode };
			}
		}
	}

	void showInfo(int x, int y) {
		std::string info;
		if (currentNode) {
			info += "Current Node: x:" + std::to_string(currentNode->pos.x) + ", y:" + std::to_string(currentNode->pos.y);
		}
		info += "\nOpenNodes Count: " + std::to_string(openNodes.getCount());
		info += "\nClosedNodes Count: " + std::to_string(closedNodes.getCount());
		DrawText(info.c_str(), x, y, 20, BLACK);
	}
};

int getParents(Node* node) {
	if (!node) {
		return -1;
	}

	int num = 0;
	while (node->parent) {
		num++;
		node = node->parent;
	}

	return num;
}

// Not worrying about efficiency here, just trying to make a demo.

int main() {
	int screenWidth = 1600;
	int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "A* Pathfinding Demo");

	//SetTargetFPS(240);

	NavGrid navGrid({ 500, 100, (float)screenWidth - 500, (float)screenHeight - 100 }, 20, 20);

	navGrid.setStartPos({ 1, 10 });
	navGrid.setGoalPos({ 18, 12 });

	AStar astar({ 1, 10 }, { 18, 12 });

	float period = 0.1;
	float timer = period;
	bool paused = true;

	while (!WindowShouldClose()) {
		// Updates
		float delta = GetFrameTime();

		if (IsKeyReleased(KEY_SPACE)) {
			paused = !paused;
		}

		if (paused) {
			if (IsKeyReleased(KEY_RIGHT)) {
				astar.update();
			}
		}
		else {
			timer -= delta;
		}

		if (timer <= 0) {
			timer = period;
			astar.update();
		}

		Node* currentNode = astar.getCurrentNode();
		Array<Node*> openNodes = astar.getOpenNodes();
		Array<Node*> closedNodes = astar.getClosedNodes();

		int parentCount = getParents(currentNode);
		

		// Drawing
		BeginDrawing();

		ClearBackground(RAYWHITE);

		navGrid.draw();

		for (int i = 0; i < openNodes.getCount(); i++) {
			navGrid.highlightCell(openNodes[i]->pos, GREEN);
		}
		for (int i = 0; i < closedNodes.getCount(); i++) {
			navGrid.highlightCell(closedNodes[i]->pos, RED);
		}
		if (currentNode) {
			Node* retrace = currentNode;
			while (retrace->parent) {
				retrace = retrace->parent;
				navGrid.highlightCell(retrace->pos, PURPLE);
			}

			navGrid.highlightCell(currentNode->pos, BLUE);
		}

		


		DrawFPS(10, 10);

		navGrid.showInfo(10, 40);
		astar.showInfo(10, 100);

		EndDrawing();
	}
}