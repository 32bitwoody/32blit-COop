#pragma once

#include "game.def"
#include <cstdint>

#include "32blit.hpp"
#include "assets.hpp"

using namespace blit;

class scoretable {
  private:
  std::string title[3] = { "Cephalopod", "Oceanaut","OPeration"};
//    JPEGImage jpeg;
// char title1[10]; // = "Cephalopod"; 
// char title2[8];  //= "Oceanaut"; 
// char title3[9]; //=   "OPeration"; 


// teletype f3 172 as backgroud e3 167 of 30ms, at 152ms cycle
uint16_t beat = 0;
uint32_t timestart = -1;

  public:
    bool firstrun;




     scoretable();
void init(uint8_t &m_gamestate);
void update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]);

void render(uint32_t time);
void render_title(void);
void render_teletype(void);


};
