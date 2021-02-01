#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <ctype.h>

#include "splash.hpp"


using namespace blit;


splash::splash(){
	printf("Splash Construct\n");
	firstrun=false;
}

void splash::init(void){
	printf("Splash Init ");
  screen.sprites = SpriteSheet::load(splash_data);
  
// configure voices

// melody channel
  channels[0].waveforms   = Waveform::TRIANGLE | Waveform::SQUARE;
  channels[0].attack_ms   = 16;
  channels[0].decay_ms    = 168;
  channels[0].sustain     = 0xafff;
  channels[0].release_ms  = 168;
  
// harmony channel
  channels[1].waveforms   = Waveform::TRIANGLE | Waveform::SQUARE;
  channels[1].attack_ms   = 32;
  channels[1].decay_ms    = 84;
  channels[1].sustain     = 0xafff;
  channels[1].release_ms  = 338;
  
// rhythm channel
  channels[2].waveforms   = Waveform::SQUARE;
  channels[2].attack_ms   = 38;
  channels[2].decay_ms    = 300;
  channels[2].sustain     = 0;
  channels[2].release_ms  = 0;

// percussion channel
  channels[3].waveforms   = Waveform::NOISE;
  channels[3].attack_ms   = 10;
  channels[3].decay_ms    = 750;
  channels[3].sustain     = 0;
  channels[3].release_ms  = 100;

}
void splash::update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]){
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
  if(time-timestart>1000 && time-timestart<1500) { audio(time);}
  if (button_p[ButtonM]==true) {
    	  printf("Menu pressed !\n");

    menupress=true;
  }
  if (button_p[ButtonH]==true) {
    homepress=true;
  }
  if (button_r[ButtonM]==true) {
    menupress=false;
  }
  if (button_r[ButtonH]==true) {
    homepress=false;
        m_gamestate = STATE_SETTING;

  }
}
//----------------------------------------------------------------------
void splash::render(uint32_t time){
// draw backdrop stretched to screen
  screen.clear();
  screen.stretch_blit(screen.sprites, Rect(0, 0, 192, 128), Rect(0, 0, 320, 240));

  if(time-timestart>1000 && time-timestart<5000) { 
    render_title();
    firstrun=true;
  }
  if(time-timestart>5000) { 
    render_teletype();
  }   
  if (menupress==true || homepress==true) {
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(0, 0, 320, 14));
    screen.pen = Pen(0, 0, 0);
    if (menupress==true) {
      screen.text("Menu pressed!", minimal_font, Point(5, 4));
    }
    if (homepress==true) {
      screen.text("Home pressed!", minimal_font, Point(5, 4));
    }
  }
}
//----------------------------------------------------------------------
void splash::render_title(void) {
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
//----------------------------------------------------------------------
void splash::render_teletype(void) {
  screen.alpha = 255;
  screen.mask = nullptr;
//  screen.pen = Pen(255, 255, 255);
  screen.pen = Pen(58, 94, 66);

//    screen.rectangle(Rect(0, 90, 320, 50));  
  Rect text_rect(5, 108, 310, 24);
  screen.rectangle(text_rect);

  screen.pen = Pen(102, 120, 114);
  std::string text = "Press A to continue";
  text = screen.wrap_text(text, text_rect.w, minimal_font, true);
  screen.text(text, minimal_font, text_rect, true, TextAlign::center_center);


  screen.pen = Pen(0, 0, 0);

}
//----------------------------------------------------------------------
void splash::audio(uint32_t time_ms) {
//  printf("audio\n");
  static uint16_t tick = 0;
  static uint16_t prev_beat = 1;
//  printf("tick:%d prev_beat:%d ",tick,prev_beat);
  beat = (tick / 5) % 10; // 100ms per beat
  tick++;
//  printf("tick:%d beat:%d\n",tick,beat);
  if (beat == prev_beat) return;
  prev_beat = beat;
  uint8_t i = 0;
//  printf("%d:%d\n",beat,notes[i][beat]);
//  for(uint8_t i = 0; i < 3; i++) {
    if(notes[i][beat] > 0) {
      channels[3].frequency = notes[i][beat];
      channels[3].volume = 256*volume[i][beat];
      channels[3].trigger_attack();
    } else if (notes[i][beat] == -1) {
      channels[3].trigger_release();
    }
    if (beat>0) {
      if(notes[i][beat-1] > 0) {
      channels[7].frequency = notes[i][beat-1];
      channels[7].volume = 256*volume[i][beat-1];
      channels[7].trigger_attack();
    } else if (notes[i][beat] == -1) {
      channels[7].trigger_release();
    }
  }
//  }
}
//----------------------------------------------------------------------
