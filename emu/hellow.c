#include "raylib.h"

static const int screenwidth = 400;
static const int screenheight = 250;


void main () {
	Rectangle rect;
	rect.width = 20;
	rect.height = 20;
	rect.x = 100;
	rect.y = 100; 
	InitWindow(screenwidth, screenheight, "hello world");
	SetTargetFPS(60);
	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawRectangleRec(rect, BLACK);
			//DrawText("Hello World!", (screenwidth/2)-100, (screenheight/2)-3, 30, LIGHTGRAY);
		EndDrawing();
	}
	CloseWindow();
}
