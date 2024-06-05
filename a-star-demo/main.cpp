#include <string>
#include <algorithm>
#include "raylib.h"
#include "NavGrid.h"
#include "Array.h"
#include "AStar.h"

// Not worrying about efficiency here, just trying to make a demo.

int main() {
	int screenWidth = 1600;
	int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "A* Pathfinding Demo");

	//SetTargetFPS(240);

	NavGrid navGrid(20, 20, { 500, 100, (float)screenWidth - 500, (float)screenHeight - 100 });

	navGrid.setStartPos(int2 { 1, 10 });
	navGrid.setGoalPos(int2 { 18, 12 });

	AStar astar(&navGrid, { 1, 10 }, { 18, 12 });

	float period = 0.1;
	float timer = period;
	bool paused = true;

	while (!WindowShouldClose()) {
		// Updates
		float delta = GetFrameTime();

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			navGrid.insertBlock((Vector2)GetMousePosition());
		}
		else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			navGrid.clearBlock(GetMousePosition());
		}

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

		// Drawing
		BeginDrawing();

		ClearBackground(RAYWHITE);

		astar.draw();
		navGrid.draw();

		DrawFPS(10, 10);

		Vector2 mouse = GetMousePosition();
		int2 mouseGrid = navGrid.findGridPos(mouse);
		std::string mouseInfo;
		mouseInfo += "mouse pos: x:" + std::to_string(mouse.x) + ", " + std::to_string(mouse.y);
		mouseInfo += "\ngrid pos: x:" + std::to_string(mouseGrid.x) + ", " + std::to_string(mouseGrid.y);
		DrawText(mouseInfo.c_str(), 10, 200, 20, BLACK);

		navGrid.showInfo(10, 40);
		astar.showInfo(10, 100);

		EndDrawing();
	}
}