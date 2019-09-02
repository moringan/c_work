#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
static const int screenw = 380;
static const int screenh = 320;

void main() {
	int delay = 10000;
	InitWindow(screenw, screenh, "image manipulation");
//	void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
	SetTargetFPS(60);
	ClearBackground(BLACK);
	while(!WindowShouldClose()) {
		for(int i = 300, x = 0; x < 300; i-=5, x+= 5) {
				BeginDrawing();
			
				ClearBackground(BLACK);
				usleep(delay);
				DrawText("hello", x, x, 33, RED);
				DrawText("hello", i, i, 33, GREEN);
				DrawText("hello", x, i, 33, YELLOW);
				DrawText("hello", i, x, 33, BLUE);
				
				EndDrawing();
		}
	}
	CloseWindow();
	
}
