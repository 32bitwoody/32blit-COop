#pragma once

#include "game.def"
#include <cstdint>

#include "32blit.hpp"
#include "assets.hpp"

using namespace blit;

class menu {
  private:
    std::string title[5] = { "Sound", "Music","Game state","Difficulty","Exit"};
    std::string extra[5][2] = {{ "On","Off" },{ "On","Off" },{ "Continue","Restart" },{ "Easy","Hard" },{ "-","-" }};
  
//  int a[3][4] = {  
//   {0, 1, 2, 3} ,   /*  initializers for row indexed by 0 */
//   {4, 5, 6, 7} ,   /*  initializers for row indexed by 1 */
//   {8, 9, 10, 11}   /*  initializers for row indexed by 2 */
//};
    uint16_t beat = 0;
    uint32_t timestart = -1;
    uint8_t option;

  public:
    bool firstrun;


     menu();
void init(uint8_t &m_gamestate);
void update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[],bool setting[] );
void update_option(uint8_t opt,bool &o);

void render(uint32_t time,bool setting[]);
void render_menu(bool setting[]);
void render_selection(bool setting[]);


};
