#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <ctype.h>

#include "scoretable.hpp"


using namespace blit;


scoretable::scoretable(){
	printf("Scores Construct\n");
	firstrun=false;
}

void scoretable::init(uint8_t &m_gamestate){
	printf("Score Init ");
  screen.sprites = SpriteSheet::load(splash_data);

}
void scoretable::update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]){
  if(timestart==-1) { timestart=time; }
//  printf("time %d\n",time);
  if (button_p[ButtonA]==true) {
	  printf("A pressed\n");
  }
  if (button_r[ButtonA]==true) {
    printf("Release: A\n");
    m_gamestate = STATE_INITPLAY;
  }
  // one sec in play crash;
//  if(time-timestart>1000 && time-timestart<1500) { audio(time);}
}
void scoretable::render(uint32_t time){
//  printf("time %d\n",time);
// Have we been here before then jump out
//  if(firstrun==true){
//   	return;
//  }
// draw backdrop stretched to screen
  screen.clear();
  screen.stretch_blit(screen.sprites, Rect(0, 0, 192, 128), Rect(0, 0, 320, 240));


 
  printf("time  %d\n",time);
  if(time-timestart>1000 && time-timestart<5000) { 
    render_title();
    firstrun=true;
  }
    if(time-timestart>5000) { 
    render_teletype();
    
  }
}
void scoretable::render_title(void) {
//  printf("Titles");
  int tt;
  Point cur;
  int X=60;
  int nm=sizeof(title[1]);
  for(int t=0;t<3; t++) {
    if(t==2) { X=X-8; }
    for(int n=0;n<12; n++) {
      if(title[1][t]==0) { break;}
//      printf("%d,",title[t][n]);
      cur=Point(X+n*16,t*16+65);
      tt=title[t][n];
      if(tt> 64 && tt< 78) { screen.sprite(Rect((tt- 65)*2, 16, 2, 2), cur); }
      if(tt> 77 && tt< 91) { screen.sprite(Rect((tt- 78)*2, 18, 2, 2), cur); }
      if(tt> 96 && tt<110) { screen.sprite(Rect((tt- 97)*2, 20, 2, 2), cur); }
      if(tt>109 && tt<123) { screen.sprite(Rect((tt-110)*2, 22, 2, 2), cur); }
    }
//    printf("\n");
  }
}
void scoretable::render_teletype(void) {

   screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(0, 108, 320, 24));
    screen.pen = Pen(0, 0, 0);
    screen.text("Press A to continue", minimal_font, Point(5, 112));
//    screen.text("OPeration", minimal_font, Point(5, 118));
//  screen.text("Test", fat_font, Rect(5, 124, 100,50));
}


