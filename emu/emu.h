const unsigned char screen_multiplier = 10; 
unsigned short opcode;		//opcode is 2 bytes
unsigned char memory[4096];
unsigned char V[16];		//Registers
unsigned short I;		//Index Register??  12bits
unsigned short pc;		//Program Counter - memory address of next instruction 
unsigned char gfx[32][64];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned short stack[16];
unsigned char chip8_fontset[80];
unsigned short sp;
unsigned char keys[16];
unsigned char draw_flag;
void initialize();
void emulate_cycle();
void load_game();
void debug_render();



