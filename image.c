#include "raylib.h"
#include <stdio.h>

static const int screenw = 600;
static const int screenh = 400;

void main() {
	InitWindow(screenw, screenh, "image manipulation");
	for(unsigned long i=0; i < 10000000000; i++);
	CloseWindow();
	
}
