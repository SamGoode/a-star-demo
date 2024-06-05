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

	SetTargetFPS(240);

	NavGrid navGrid(20, 20, { 500, 100, (float)screenWidth - 500, (float)screenHeight - 100 });

	navGrid.setStartPos(int2 { 1, 10 });
	navGrid.setGoalPos(int2 { 18, 12 });

	AStar astar(&navGrid);

	float period = 0.1;
	float timer = period;
	bool paused = true;

	while (!WindowShouldClose()) {
		// Updates
		float delta = GetFrameTime();

		if (!astar.running) {
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
				navGrid.insertBlock(GetMousePosition());
			}
			else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
				navGrid.clearBlock(GetMousePosition());
			}

			if (IsKeyReleased(KEY_S)) {
				navGrid.setStartPos(GetMousePosition());
			}
			if (IsKeyReleased(KEY_G)) {
				navGrid.setGoalPos(GetMousePosition());
			}
		}

		if (IsKeyReleased(KEY_R)) {
			astar.restart();
		}

		if (IsKeyReleased(KEY_SPACE)) {
			timer = period;
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
		std::string mouseInfo = "Mouse pos:";
		mouseInfo += "\nscreen: x:" + std::to_string(mouse.x) + ", " + std::to_string(mouse.y);
		mouseInfo += "\ngrid: x:" + std::to_string(mouseGrid.x) + ", " + std::to_string(mouseGrid.y);
		DrawText(mouseInfo.c_str(), 10, 200, 20, BLACK);

		std::string status = "Status: ";
		if (paused) {
			status += "paused\n";
		}
		else {
			status += "playing\n";
		}
		status += "Pathfinding: ";
		if (astar.running) {
			status += "running\n";
		}
		else {
			status += "done\n";
			
			if (astar.pathFound) {
				status += "Path found";
			}
			else {
				status += "Path not found";
			}
		}

		DrawText(status.c_str(), 10, 400, 20, BLACK);

		std::string explanation;
		explanation += "Blue is Start cell\n";
		explanation += "Orange is Goal cell\n";
		explanation += "Black cells are obstacles/walls\n";
		explanation += "Green cells are ready for searching\n";
		explanation += "Red cell are already searched\n";
		explanation += "Purple cells show current shortest path";
		DrawText(explanation.c_str(), screenWidth - 450, 100, 20, BLACK);

		std::string controls;
		controls += "Space is play/pause\n";
		controls += "Right arrow is step forward\n";
		controls += "R key is restart\n\n";
		controls += "Immediately after restart,\nthese controls can be used:\n";
		controls += "S key changes start cell to hovered cell\n";
		controls += "G key changes goal cell to hovered cell\n";
		controls += "Left click places wall cell\n";
		controls += "Right click removes wall cell\n";
		DrawText(controls.c_str(), screenWidth - 450, 350, 20, BLACK);

		navGrid.showInfo(10, 40);
		astar.showInfo(10, 100);

		EndDrawing();
	}
}