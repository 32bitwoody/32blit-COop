#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <ctype.h>

#include "menu.hpp"


using namespace blit;

//----------------------------------------------------------------------
menu::menu(){
	printf("Menu Construct\n");
	firstrun=false;
}
//----------------------------------------------------------------------
void menu::init(uint8_t &m_gamestate){
	printf("Menu Init ");
  screen.sprites = SpriteSheet::load(splash_data);
  option=0;
}
//----------------------------------------------------------------------
void menu::update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[], bool setting[]){
  if(timestart==-1) { timestart=time; }
//  printf("time %d\n",time);
  if (button_r[DpadUp]==true) {
	  if(option>0) { option--; }
    button_r[DpadUp]=false;
    printf("Option %d\n",option);
  }
  if (button_r[DpadDown]==true) {
    if(option<5) { option++; }
    button_r[DpadDown]=false;
    printf("Option %d\n",option);
  }
  if (button_r[ButtonH]==true) {
    m_gamestate = STATE_PLAY;
  }
//        printf("Menu pre setting %d\n",setting[0]);

      
//if button x
  if (button_r[ButtonX]==true) {
    button_r[ButtonX]=false;
    printf("Menu X released\n");
  // case option
  printf("Changing option %d",option);
  update_option(option,setting[option]);
  //change element of setting array
  }
//        printf("Menu post setting %d\n",setting[0]);

}
//----------------------------------------------------------------------
void menu::update_option(uint8_t opt,bool &o) {
  printf("Option is %d\n",opt);
  switch( opt ) {
  case SETT_SOUND:      /* Show the user a simple splash screen. */
    printf("Switch sound\n");
    o = !o;
    break;
 
  case SETT_MUSIC:
  
  break;      
  case SETT_RESTART:
  printf("Switch restart\n");
    o = !o;
  break;       
  case SETT_DIFFICULTY:
  
  break; 
  case SETT_EXIT:
      printf("Exit !\n");
//exit_game = true;
   exit(0);
  break;    
}
}
//----------------------------------------------------------------------
void menu::render(uint32_t time,bool setting[]){
  screen.clear();
  screen.stretch_blit(screen.sprites, Rect(0, 0, 192, 128), Rect(0, 0, 320, 240));
//  printf("time  %d\n",time);

    render_menu(setting);
    render_selection(setting);
    
  
}
//----------------------------------------------------------------------
void menu::render_menu(bool setting[]) {
//  printf("Menu\n");
  screen.alpha = 255;
  screen.mask = nullptr;
  
  screen.pen = Pen(58, 94, 66);
  Rect text_rect(5, 5, 310, 24);
  screen.rectangle(text_rect);

  screen.pen = Pen(102, 120, 114);
  std::string text = "Menu";
  text = screen.wrap_text(text, text_rect.w, minimal_font, true);
  screen.text(text, outline_font, text_rect, true, TextAlign::center_center);
  

  screen.pen = Pen(0, 0, 0);

  for(int t=0;t<5; t++) {
//    printf("%.4s\n",title[t]);



    screen.text(title[t]+extra[t][setting[t]], minimal_font, Point(20, 50+t*10));

  }  
    screen.pen = Pen(0, 0, 0);

}
//----------------------------------------------------------------------
void menu::render_selection(bool setting[]) {

   screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(58, 94, 66);
    screen.rectangle(Rect(5, 48+option*10, 310, 12));
    screen.pen = Pen(0, 0, 0);
    
    screen.text(title[option]+" "+extra[option][setting[option]], minimal_font, Point(21, 50+option*10));

}


