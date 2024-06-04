#include "NavGrid.h"

NavGrid::NavGrid(Vector4 _bounds, int _width, int _height) {
	bounds = _bounds;
	width = _width;
	height = _height;
	cellWidth = (bounds.z - bounds.x) / width;
	cellHeight = (bounds.w - bounds.y) / height;
	startPos = { -1, -1 };
	goalPos = { -1, -1 };
}

void NavGrid::highlightCell(int2 pos, Color color) {
	DrawRectangle(bounds.x + pos.x * cellWidth, bounds.y + pos.y * cellHeight, cellWidth, cellHeight, color);
}

void NavGrid::draw() {
	if (valid(goalPos)) {
		highlightCell(goalPos, ORANGE);
	}

	if (valid(startPos)) {
		//highlightCell(startPos, GREEN);
	}

	DrawRectangleLines(bounds.x, bounds.y, bounds.z - bounds.x, bounds.w - bounds.y, BLACK);
	for (int i = 1; i < width; i++) {
		DrawLine(bounds.x + (i * cellWidth), bounds.y, bounds.x + (i * cellWidth), bounds.w, BLACK);
	}
	for (int i = 1; i < height; i++) {
		DrawLine(bounds.x, bounds.y + (i * cellHeight), bounds.z, bounds.y + (i * cellHeight), BLACK);
	}
}

void NavGrid::showInfo(int x, int y) {
	std::string info;
	info += "Start pos: x:" + std::to_string(startPos.x) + ", y:" + std::to_string(startPos.y);
	info += "\nGoal pos: x:" + std::to_string(goalPos.x) + ", y:" + std::to_string(goalPos.y);
	DrawText(info.c_str(), x, y, 20, BLUE);
}