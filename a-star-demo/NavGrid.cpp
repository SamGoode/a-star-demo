#include "NavGrid.h"
#include <string>


NavGrid::NavGrid(int _width, int _height, Vector4 _bounds) {
	width = _width;
	height = _height;

	bounds = _bounds;
	cellWidth = (bounds.z - bounds.x) / width;
	cellHeight = (bounds.w - bounds.y) / height;
	startPos = { -1, -1 };
	goalPos = { -1, -1 };

	bitmap = new bool[width * height] {false};

	//for (int x = 5; x < 15; x++) {
	//	//bitmap[x + 15 * width] = true;
	//	insertBlock(int2 { x, 15 });
	//}
	//for (int y = 5; y < 15; y++) {
	//	//bitmap[10 + y * width] = true;
	//	insertBlock(int2{ 10, y });
	//}
}

void NavGrid::highlightCell(int2 pos, Color color) {
	if (!valid(pos)) {
		return;
	}

	DrawRectangle(bounds.x + pos.x * cellWidth, bounds.y + pos.y * cellHeight, cellWidth, cellHeight, color);
}

void NavGrid::draw() {
	if (valid(goalPos)) {
		highlightCell(goalPos, ORANGE);
	}

	if (valid(startPos)) {
		highlightCell(startPos, BLUE);
	}

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (bitmap[x + y * width]) {
				highlightCell({ x, y }, BLACK);
			}
		}
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