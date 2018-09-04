/*	example code for cc65, for NES
 *  move some sprites with the controllers
 *	using neslib
 *	Doug Fraker 2018
 */	
 


#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "Sprites.h" // holds our metasprite data
#include "collide.h"


	
	
void main (void) {
	
	ppu_off(); // screen off
	
	// load the palettes
	pal_bg(palette_bg);
	pal_spr(palette_sp);
	
	
	// use the second set of tiles for sprites
	// both bg and sprites are set to 0 by default
	bank_spr(1);
	
	set_scroll_y(0xff); //shift the bg down 1 pixel
	
	draw_bg();

	// turn on screen
	// ppu_on_all(); //already done in draw_bg()
	
	

	while (1){
		// infinite loop
		ppu_wait_nmi(); // wait till beginning of the frame
		// the sprites are pushed from a buffer to the OAM during nmi
		
		pad1 = pad_poll(0); // read the first controller
		pad1_new = get_pad_new(0); // newly pressed button. do pad_poll first
		
		movement();
		draw_sprites();
		check_start();
	}
}



void draw_bg(void){
	ppu_off(); // screen off
	
	p_maps = All_Collision_Maps[which_bg];
	// copy the collision map to c_map
	memcpy (c_map, p_maps, 240);
	
	// this sets a start position on the BG, top left of screen
	vram_adr(NAMETABLE_A);
	
	// draw the tiles
	for(temp_y = 0; temp_y < 15; ++temp_y){
		for(temp_x = 0; temp_x < 16; ++temp_x){
			temp1 = (temp_y << 4) + temp_x;

			if(c_map[temp1]){
				vram_put(0x10); // wall
				vram_put(0x10);
			}
			else{
				vram_put(0); // blank
				vram_put(0);
			}
		}
		
		// do twice
		for(temp_x = 0; temp_x < 16; ++temp_x){
			temp1 = (temp_y << 4) + temp_x;

			if(c_map[temp1]){
				vram_put(0x10); // wall
				vram_put(0x10);
			}
			else{
				vram_put(0); // blank
				vram_put(0);
			}
		}
	}
	
	ppu_on_all(); // turn on screen
}



void draw_sprites(void){
	// clear all sprites from sprite buffer
	oam_clear();

	// reset index into the sprite buffer
	sprid = 0;
	
	// draw 1 metasprite
	sprid = oam_meta_spr(BoxGuy1.X, BoxGuy1.Y, sprid, YellowSpr);
}
	
	
	
void movement(void){
	if(pad1 & PAD_LEFT){
		BoxGuy1.X -= 1;
	}
	else if (pad1 & PAD_RIGHT){
		BoxGuy1.X += 1;
	}
	
	bg_collision((char *)&BoxGuy1);
	if(collision_R) BoxGuy1.X -= 1;
	if(collision_L) BoxGuy1.X += 1;
	
	if(pad1 & PAD_UP){
		BoxGuy1.Y -= 1;
	}
	else if (pad1 & PAD_DOWN){
		BoxGuy1.Y += 1;
	}
	
	bg_collision((char *)&BoxGuy1);
	if(collision_D) BoxGuy1.Y -= 1;
	if(collision_U) BoxGuy1.Y += 1;
}	



void bg_collision(char * object){
	// sprite collision with backgrounds
	// object expected to have first 4 bytes as x,y,width,height
	// casting to char* so this function could work for any sized structs
	collision_L = 0;
	collision_R = 0;
	collision_U = 0;
	collision_D = 0;
	
	temp1 = object[0]; // left side
	temp2 = temp1 + object[2]; // right side
	temp3 = object[1]; // top side
	temp4 = temp3 + object[3]; // bottom side
	
	if(temp3 >= 0xf0) return;
	// y out of range
	
	coordinates = (temp1 >> 4) + (temp3 & 0xf0); // upper left
	if(c_map[coordinates]){ // find a corner in the collision map
		++collision_L;
		++collision_U;
	}
	
	coordinates = (temp2 >> 4) + (temp3 & 0xf0); // upper right
	if(c_map[coordinates]){
		++collision_R;
		++collision_U;
	}
	
	if(temp4 >= 0xf0) return;
	// y out of range
	
	coordinates = (temp1 >> 4) + (temp4 & 0xf0); // bottom left
	if(c_map[coordinates]){
		++collision_L;
		++collision_D;
	}
	
	coordinates = (temp2 >> 4) + (temp4 & 0xf0); // bottom right
	if(c_map[coordinates]){
		++collision_R;
		++collision_D;
	}
}



void check_start(void){
	// if START is pressed, load another background
	if(pad1_new & PAD_START){
		++which_bg;
		if(which_bg >= 4) which_bg = 0;
		draw_bg();
	}	
}