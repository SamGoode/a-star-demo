#pragma once
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
	bool* bitmap;

public:
	NavGrid(int _width, int _height, Vector4 _bounds);

	~NavGrid() { delete[] bitmap; }

	int2 findGridPos(Vector2 screenPos) { return { (int)((screenPos.x - bounds.x) / cellWidth), (int)((screenPos.y - bounds.y) / cellHeight) }; }

	bool valid(int2 pos) { return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height; }
	bool isBlocked(int2 pos) { if (!valid(pos)) { return true; } return bitmap[pos.x + pos.y * width]; }
	
	int2 getStartPos() { return startPos; };
	int2 getGoalPos() { return goalPos; }
	void setStartPos(int2 pos) { if (!valid(pos)) { return; } startPos = pos; }
	void setGoalPos(int2 pos) { if (!valid(pos)) { return; } goalPos = pos; }
	void insertBlock(int2 pos) { if (!valid(pos)) { return; } bitmap[pos.x + pos.y * width] = 1; }
	void clearBlock(int2 pos) { if (!valid(pos)) { return; } bitmap[pos.x + pos.y * width] = 0; }

	void setStartPos(Vector2 vec) { int2 pos = findGridPos(vec); if (isBlocked(pos) || pos == goalPos) { return; } startPos = pos; }
	void setGoalPos(Vector2 vec) { int2 pos = findGridPos(vec); if (isBlocked(pos) || pos == startPos) { return; } goalPos = pos; }
	void insertBlock(Vector2 vec) { int2 pos = findGridPos(vec); if (!valid(pos) || pos == startPos || pos == goalPos) { return; } bitmap[pos.x + pos.y * width] = 1; }
	void clearBlock(Vector2 vec) { int2 pos = findGridPos(vec); if (!valid(pos) || pos == startPos || pos == goalPos) { return; } bitmap[pos.x + pos.y * width] = 0; }

	void highlightCell(int2 pos, Color color) { if (!valid(pos)) { return; } DrawRectangle(bounds.x + pos.x * cellWidth, bounds.y + pos.y * cellHeight, cellWidth, cellHeight, color); }
	void draw();
	void showInfo(int x, int y);
};