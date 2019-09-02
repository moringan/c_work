// A Chip-8 emulator (interpreter)
// by Mike Stapleton August 2019

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include "./emu.h"


unsigned char chip8_fontset[80] =
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void initialize() {
	pc = 0x200;
	opcode = 0x0;
	I = 0x0;
	sp = 0x0;
	draw_flag = 1;
	delay_timer = 0x0;
	sound_timer = 0x0;
	for(int i=0; i<16; i++)
		V[i] = 0x0;
	for(int i=0; i<16; i++)
		stack[i] = 0x0;
	for(int i=0; i<16; i++)
		keys[i] = 0x0;
	for(int i=0; i<4096; i++)
		memory[i] = 0x0;
	for(int i=0; i<32; i++)
		for(int j=0; j<64; j++)
			gfx[i][j] = 0x0;
	for(int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];	
	srand(time(NULL));
}

// fetch, decode and execute opcodes
void emulate_cycle() {
	//fetch opcode
	opcode = (memory[pc] << 8) | memory[pc+1];

	//decode
	switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x0000:
					draw_flag = 1;
					for(int i = 0; i< 32; i++)
						for(int j = 0; j < 64; j++)
							gfx[i][j] = 0x0;
					pc += 2;					
					break;
				case 0x000E:	
					sp--;
					pc = stack[sp];
					pc += 2;
					break;
				default:
					printf("Unknown opcode\n");
			
			}
			break;
			
		case 0x1000:
			pc = opcode & 0x0FFF;
			break;
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = opcode & 0x0FFF;
			break;
		case 0x3000:
			if ( V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF) )
				pc += 4;
			else
				pc += 2;
			break;
		case 0x4000:
			if ( V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF) )
				pc += 4;
			else
				pc += 2;
			break;
		case 0x5000:
			if ( V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4] )
				pc += 4;
			else
				pc += 2;
			break;
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			pc += 2;
			break;
		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
			pc += 2;
			break;
		case 0x8000:
			switch (opcode & 0x000F) {
				case 0x0000:
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0001:
					V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0 >> 4)];
					pc += 2;
					break;
				case 0x0002:
					V[(opcode & 0x0F00) >> 8] &=  V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;

				case 0x0003:
					V[(opcode & 0x0F00) >> 8] ^=  V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0004:
					if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
						V[0xF] = 1; //carry
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0005: 
					if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						V[0xF] = 0; //borrow
					else
						V[0xF] = 1;
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
					break;
				case 0x0006:
					V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1; 
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
					break;
				case 0x0007:
					if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
						V[0xF] = 0; //borrow
					else
						V[0xF] = 1;					
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x000E:
					V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7; 
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
					break;
				default:
					printf("Unknown opcode\n");
			
			}
			break;
			
		case 0x9000:
			if ( V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4] )
				pc += 4;
			else
				pc += 2;

			break;
		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000:
			pc = V[0] + (opcode & 0x0FFF);			
			break;
		case 0xC000: 
			V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & (rand() % 0xFF);
			pc +=2;
			break;
		case 0xD000: 
		{ //draw sprite
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short rows = opcode & 0x000F;
			unsigned short pixel;
			
			V[0xF] = 0;
			for(int i = 0; i< rows; i++) {
				pixel = memory[I + i];
				for(int j = 0; j< 8; j++) {	
					unsigned short posX = (x+j) % 64;
					unsigned short posY = (y+i) % 32;
					if( (pixel & (0x80 >> j)) != 0) {
						if(gfx[posY][posX] == 1)
							V[0xF] = 1;
						gfx[posY][posX] ^= 1;
					}
				}
					
			}
			draw_flag = 1;
			pc += 2;
			
		}	
		break;	
		
			 
		case 0xE000:	
			switch (opcode & 0x00FF) {
				case 0x009E: //EX9E
					if(keys[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc += 2;
					break;
				case 0x00A1:
					if(keys[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc += 2;
					break;
				default:
					printf("unknown opcode\n");
			
			}
			break;
			
			
		case 0xF000:
			switch (opcode & 0x00FF) 
			{
				case 0x0007:
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
					break;
				case 0x000A: 
				{
					unsigned char key_pressed = 0;
					for(int i=0; i<16; i++) 
						if(keys[i] != 0) {
							key_pressed = 1;
							V[(opcode & 0x0F00) >> 8] = i;
						}
					if(!key_pressed) 
						return;
					pc += 2;
					
				}
				break;
				
				case 0x0015:
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x0018:
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x001E:
					if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				case 0x0029:
					I = V[(opcode & 0x0F00) >> 8] * 0x5;
					pc += 2;
					break;
				case 0x0033:
					memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
					break;
				case 0x0055: {
					int last = (opcode & 0x0F00) >> 8;
					for (int i = 0; i <= last; i++)
						memory[I + i] = V[i];
		//Original interpreter
//					I += (last + 1);
					pc += 2;
					
				}	
				break;				
					
				case 0x0065: {
					int last = (opcode & 0x0F00) >> 8;
					for (int i = 0; i <= last; i++)
						V[i] = memory[I + i];
		//Original interpreter
//					I += (last + 1);
					pc += 2;
					
				}					
				break;
				
				default:
					printf("unknown opcode\n");
			
			}
			break;

		default:
			printf("uknown opcode\n");
			
	}
	
	
	// Update timers
	if(delay_timer > 0)
		--delay_timer;
	if(sound_timer > 0) {
		--sound_timer;
		if(sound_timer == 1);
      			printf("BEEP!\n");
		
	}  

}

void set_keys() {}

void load_game(char *name) {
	FILE *fp;
	if( (fp = fopen(name, "rb")) == NULL)
		printf("Can't open file\n");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	char *buffer = (char *)malloc(sizeof(char) * size);
	size_t result = fread(buffer, 1, size, fp);
	if(result != size) 
		printf("Read error\n");
	for(int i=0; i < size; i++) {
		memory[i + 512] = buffer[i];
	}
	fclose(fp);
	free(buffer);
	
}

void print_cpu() {
	printf("pc: %x\n", pc);
	printf("opcode: %x\n", opcode);
	printf("I: %x\n", I);
	printf("sp: %x\n", sp);
	printf("stack: %x\n", stack[sp-1]);
	printf("\n");

}


void check_keyboard()
{
	if(IsKeyDown(KEY_F1))	keys[0x1] = 1;
	if(IsKeyDown(KEY_F2))	keys[0x2] = 1;
	if(IsKeyDown(KEY_F3))	keys[0x3] = 1;
	if(IsKeyDown(KEY_F4))	keys[0xC] = 1;

	if(IsKeyDown(KEY_Q))	keys[0x4] = 1;
	if(IsKeyDown(KEY_W))	keys[0x5] = 1;
	if(IsKeyDown(KEY_E))	keys[0x6] = 1;
	if(IsKeyDown(KEY_R))	keys[0xD] = 1;

	if(IsKeyDown(KEY_A))	keys[0x7] = 1;
	if(IsKeyDown(KEY_S))	keys[0x8] = 1;
	if(IsKeyDown(KEY_D))	keys[0x9] = 1;
	if(IsKeyDown(KEY_F))	keys[0xE] = 1;

	if(IsKeyDown(KEY_Z))	keys[0xA] = 1;
	if(IsKeyDown(KEY_X))	keys[0x0] = 1;
	if(IsKeyDown(KEY_C))	keys[0xB] = 1;
	if(IsKeyDown(KEY_V))	keys[0xF] = 1;



	if(IsKeyUp(KEY_F1))	keys[0x1] = 0;
	if(IsKeyUp(KEY_F2))	keys[0x2] = 0;
	if(IsKeyUp(KEY_F3))	keys[0x3] = 0;
	if(IsKeyUp(KEY_F4))	keys[0xC] = 0;

	if(IsKeyUp(KEY_Q))	keys[0x4] = 0;
	if(IsKeyUp(KEY_W))	keys[0x5] = 0;
	if(IsKeyUp(KEY_E))	keys[0x6] = 0;
	if(IsKeyUp(KEY_R))	keys[0xD] = 0;

	if(IsKeyUp(KEY_A))	keys[0x7] = 0;
	if(IsKeyUp(KEY_S))	keys[0x8] = 0;
	if(IsKeyUp(KEY_D))	keys[0x9] = 0;
	if(IsKeyUp(KEY_F))	keys[0xE] = 0;

	if(IsKeyUp(KEY_Z))	keys[0xA] = 0;
	if(IsKeyUp(KEY_X))	keys[0x0] = 0;
	if(IsKeyUp(KEY_C))	keys[0xB] = 0;
	if(IsKeyUp(KEY_V))	keys[0xF] = 0;

	//printf("Press key %c\n", key);
}

void draw_graphics() {
	Rectangle rect;
	rect.width = rect.height = screen_multiplier;
	ClearBackground(BLACK);
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			rect.x = x * screen_multiplier;
			rect.y = y * screen_multiplier;
			if(gfx[y][x] != 0x0) 
				DrawRectangleRec(rect, RAYWHITE);
		}
	}
}

void debug_render() {

	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(gfx[y][x] == 0x0) 
				printf(" ");
			else 
				printf("X");
		}
		printf("\n");
	}
	printf("\n");
}


int main(int argc, char** argv) {

	if(argc != 2) {
		printf("need a filename\n");
		exit(1);
	}
	initialize();
	load_game(argv[1]);

	SetTargetFPS(60);  
	InitWindow(64 * screen_multiplier, 32 * screen_multiplier, "Chip-8 Emulator");
	while(!WindowShouldClose()) {
		check_keyboard();		
		emulate_cycle();
		if(draw_flag) {
			BeginDrawing();
				draw_graphics();
			EndDrawing();
//			debug_render();
			draw_flag = 0;
		}
	}
	CloseWindow();
	return 0;
}
