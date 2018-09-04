

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
unsigned char sprid; // remember the index into the sprite buffer
unsigned char pad1;
unsigned char pad1_new;
unsigned char collision;
unsigned char collision_L;
unsigned char collision_R;
unsigned char collision_U;
unsigned char collision_D;
unsigned char which_bg;
const unsigned char * p_maps;
unsigned char coordinates;
unsigned char temp1;
unsigned char temp2;
unsigned char temp3;
unsigned char temp4;
unsigned char temp_x;
unsigned char temp_y;


#pragma bss-name(push, "BSS")

unsigned char c_map[240];
// collision map 


struct BoxGuy {
	unsigned char X;
	unsigned char Y;
	unsigned char width;
	unsigned char height;
};

struct BoxGuy BoxGuy1 = {64,80,15,15};








// these are the 4 backgrounds
// collision data, made by exporting csv from Tiled, and slight modification by CSV2C.py
#include "CSV/c1.c" // called c1
#include "CSV/c2.c" // called c2
#include "CSV/c3.c" // called c3
#include "CSV/c4.c" // called c4

const unsigned char * const All_Collision_Maps[] = {c1,c2,c3,c4};





const unsigned char palette_bg[]={
0x0f, 0x00, 0x10, 0x30, // black, gray, lt gray, white
0,0,0,0,
0,0,0,0,
0,0,0,0
}; 

const unsigned char palette_sp[]={
0x0f, 0x0f, 0x0f, 0x28, // black, black, yellow
0x0f, 0x0f, 0x0f, 0x12, // black, black, blue
0,0,0,0,
0,0,0,0
}; 



// PROTOTYPES
void draw_bg(void);
void draw_sprites(void);
void movement(void);	
void bg_collision(char * object);
void check_start(void);

