#include "AStar.h"
#include <string>
#include <algorithm>
#include <functional>

AStar::AStar(NavGrid* _grid, int2 _startPos, int2 _destPos) {
	grid = _grid;

	startPos = _startPos;
	destPos = _destPos;

	startNode = new Node{ startPos, 0, getDist(startPos, destPos), nullptr };
	currentNode = nullptr;

	container = Array<Node*>(0, 16);

	openNodes = Array<Node*>(0, 16);
	closedNodes = Array<Node*>(0, 16);

	pathFound = false;
	noPathFound = false;

	container.append(startNode);
	openNodes.append(startNode);
}

AStar::~AStar() {
	for (int i = 0; i < container.getCount(); i++) {
		delete container[i];
	}
}

bool AStar::validPos(int2 pos) {
	if (!grid->valid(pos)) {
		return false;
	}

	return !grid->isBlocked(pos);
}

float AStar::getDist(int2 pos1, int2 pos2) {
	float dx = pos1.x - pos2.x;
	float dy = pos1.y - pos2.y;
	return sqrt(dx * dx + dy * dy);
}

Array<int2> AStar::getAdjPositions(int2 pos) {
	Array<int2> adjPositions(0, 8);

	int2 offsets[8] = {
		{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
	};

	for (int i = 0; i < 8; i++) {
		int2 adjPos = pos + offsets[i];
		if (validPos(adjPos)) {
			adjPositions.append(adjPos);
		}
	}

	return adjPositions;
}

int AStar::searchOpenNodes(int2 pos) {
	for (int i = 0; i < openNodes.getCount(); i++) {
		if (openNodes[i]->pos == pos) {
			return i;
		}
	}

	return -1;
}

int AStar::searchClosedNodes(int2 pos) {
	for (int i = 0; i < closedNodes.getCount(); i++) {
		if (closedNodes[i]->pos == pos) {
			return i;
		}
	}

	return -1;
}

void AStar::restart() {
	for (int i = 0; i < container.getCount(); i++) {
		delete container[i];
	}

	startNode = new Node{ startPos, 0, getDist(startPos, destPos), nullptr };
	currentNode = nullptr;

	container = Array<Node*>(0, 16);

	openNodes = Array<Node*>(0, 16);
	closedNodes = Array<Node*>(0, 16);

	pathFound = false;
	noPathFound = false;

	container.append(startNode);
	openNodes.append(startNode);
}

void AStar::update() {
	if (pathFound || noPathFound) {
		return;
	}

	if (openNodes.getCount() == 0) {
		noPathFound = false;
		return;
	}

	std::sort(openNodes.begin(), openNodes.end(), [](Node* a, Node* b) -> bool {return a->fCost() < b->fCost(); });
	std::sort(openNodes.begin(), openNodes.end(), [](Node* a, Node* b) -> bool {return a->hCost < b->hCost; });

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

		float gCost = currentNode->gCost + getDist(currentNode->pos, adjPositions[i]);
		float hCost = getDist(adjPositions[i], destPos);
		float fCost = gCost + hCost;

		int index = searchOpenNodes(adjPositions[i]);

		if (index == -1) {
			container.append(new Node{ adjPositions[i], gCost, hCost, currentNode });
			openNodes.append(container[container.getCount() - 1]);
		}
		else if (fCost < openNodes[index]->fCost()) {
			*openNodes[index] = { adjPositions[i], gCost, hCost, currentNode };
		}
	}
}

void AStar::draw() {
	for (int i = 0; i < openNodes.getCount(); i++) {
		grid->highlightCell(openNodes[i]->pos, GREEN);
	}
	for (int i = 0; i < closedNodes.getCount(); i++) {
		grid->highlightCell(closedNodes[i]->pos, RED);
	}
	if (currentNode) {
		Node* retrace = currentNode;
		while (retrace->parent) {
			retrace = retrace->parent;
			grid->highlightCell(retrace->pos, PURPLE);
		}

		grid->highlightCell(currentNode->pos, BLUE);
	}
}

void AStar::showInfo(int x, int y) {
	std::string info;
	if (currentNode) {
		info += "Current Node: x:" + std::to_string(currentNode->pos.x) + ", y:" + std::to_string(currentNode->pos.y);
	}
	info += "\nOpenNodes Count: " + std::to_string(openNodes.getCount());
	info += "\nClosedNodes Count: " + std::to_string(closedNodes.getCount());
	DrawText(info.c_str(), x, y, 20, BLACK);
}