#pragma once
#include <string>
#include "raymath.h"
#include "int2.h"

class NavGrid {
private:
	Vector4 bounds;
	int width;
	int height;
	float cellWidth;
	float cellHeight;
	int2 startPos;
	int2 goalPos;

public:
	NavGrid(Vector4 _bounds, int _width, int _height);

	void setStartPos(int2 pos) { startPos = pos; }

	void setGoalPos(int2 pos) { goalPos = pos; }

	bool valid(int2 pos) { return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height; }

	void highlightCell(int2 pos, Color color);

	void draw();

	void showInfo(int x, int y);
};