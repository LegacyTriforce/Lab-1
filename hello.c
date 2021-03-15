
/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/

#include "neslib.h"
#include "vrambuf.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"
//#link "vrambuf.c"

#define TILE 0xd8
#define ATTR 0
#define FLIP OAM_FLIP_H

const unsigned char metaspriteR[]={
        0,      0,      TILE+0,   ATTR,
        0,      8,      TILE+1,   ATTR,
        8,      0,      TILE+2,   ATTR,
        8,      8,      TILE+3,   ATTR, 
        128};

const unsigned char metaspriteL[]={
        0,      0,      TILE+2,   FLIP, 
        0,      8,      TILE+3,   FLIP, 
        8,      0,      TILE+0,   FLIP, 
        8,      8,      TILE+1,   FLIP, 
        128};

// main function, run after console reset
void main(void) 
{
  unsigned char x = 0;
  unsigned char is_right = 0;
  
  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white
  
  pal_col(17, 0x15);
  pal_col(18, 0x28);
  pal_col(19, 0x30);

  // write text to name table
  vram_adr(NTADR_A(2,2));		// set address
  vram_write("This is", 7);	// write bytes to video RAM
  vram_adr(NTADR_A(2,3));
  vram_write("Tyler Swearingen's", 18);
  vram_adr(NTADR_A(2,4));
  vram_write("first NES 'Game'!", 17);
  
  vram_adr(NTADR_A(1, 22));
  vram_fill(0x0e,30);
  
  vram_adr(NTADR_A(29, 20));
  vram_put(0xc4);
  vram_adr(NTADR_A(30, 20));
  vram_put(0xc6);
  vram_adr(NTADR_A(29, 21));
  vram_put(0xc5);
  vram_adr(NTADR_A(30, 21));
  vram_put(0xc7);
  
  // enable PPU rendering (turn on screen)
  ppu_on_all();
  vrambuf_clear();
  set_vram_update(updbuf);

  // infinite loop
  while (1) 
  {
    char cur_oam = 0;
    char door = 0;
    
    if(is_right == 0)
    {
      x += 1;
      cur_oam = oam_meta_spr(x, 160, cur_oam, metaspriteR);
    }
    else
    {
      x -= 1;
      cur_oam = oam_meta_spr(x, 160, cur_oam, metaspriteL);
    }
    
    if(x == 236)
    {
      is_right = 1;
    }
    else if(x == 8)
    {
      is_right = 0;
    }
    
    if(x >= 228)
    {
      vrambuf_put(NTADR_A(2, 5), "On the door!", 12);
    }
    else
    {
      vrambuf_put(NTADR_A(2, 5), "           ", 12);
    }
    
    vrambuf_flush();
  }
}
