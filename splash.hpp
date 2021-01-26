#pragma once

#include "game.def"
#include <cstdint>

#include "32blit.hpp"
#include "assets.hpp"

using namespace blit;

class splash {
  private:
    std::string title[3] = { "Cephalopod", "Oceanaut","OPeration"};

    int16_t notes[2][10] = {
  { // melody notes
    99,198, 396, 0, -1, 0, 0, 0, 0, 0, 
  },
  { // melody notes
    147, 0, 0, 0, 0, 0, 0, 0, 175, 0, 
  }
  };

    int16_t volume[2][10] = {
  { // melody notes
    255,63, 16, 0, -1, 0, 0, 0, 0, 0, 
  },
  { // melody notes
    147, 0, 0, 0, 0, 0, 0, 0, 175, 0, 
  }
  };
// teletype f3 172 as backgroud e3 167 of 30ms, at 152ms cycle
    uint16_t beat = 0;
    uint32_t timestart = -1;

    bool menupress;
    bool homepress;

  public:
    bool firstrun;




     splash();
void init(void);
void update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]);
void audio(uint32_t time_ms);


void render(uint32_t time);
void render_title(void);
void render_teletype(void);


};
