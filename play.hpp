#pragma once

#include "game.def"
#include <cstdint>

#include "32blit.hpp"
#include "assets.hpp"

using namespace blit;

class play {
  private:
  
  struct ScoreSave {
    char name[10];
    uint16_t score;
    uint16_t highscore;
  };

  struct GameSave {
    char name[10];
    uint8_t level;
    uint8_t lives;
    uint8_t horde[40]= {1};
    uint8_t building[48]= {1};
    uint8_t turret;
    uint8_t bombcount;
  };


  ScoreSave score_data;
  GameSave game_data;

  
  bool gameover;
  bool gameack;
  bool lostlife;
    
    uint32_t hordetime[40]= {0};
    uint8_t counthorde;
    Point bomb[10];
    uint8_t bombcount;

  	

    Point seperate;
  	uint32_t prevtime;
    uint32_t levelstart;


    Point hordemove;
    uint8_t hordespeed;
    uint8_t hordesprite;
    Point direction;
    uint8_t targetting;

    Point missile; 
    
    // invader walk 259hz of noise for 80ms every 570ms
// invader death 859hz of saw for 80ms &
// on channel 0
//                1728hz of saw for 245ms
//mothership     810hz of triangle or wave continuous
// on channel 1
// air support
// on channel 2
// turret
// on channel 3
// buildings
// on channel 4
//...
// on channel 7
    int16_t notes[2][10] = {
  { // melody notes
    259, -1,0, 0, 0, 0, 0, 0, 0, 0, 
  },
  { // melody notes
    147, 0, 0, 0, 0, 0, 0, 0, 175, 0, 
  }
};

int16_t volume[2][10] = {
  { // melody notes
    255, -1,0, 0, 0, 0, 0, 0, 0, 0, 
  },
  { // melody notes
    147, 0, 0, 0, 0, 0, 0, 0, 175, 0, 
  }
};
uint16_t beat = 0;
uint16_t tick = 0;
  public:
    bool firstrun;
   bool gamedataload; 




     play();
void init(uint8_t &m_gamestate);
void update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]);
void render(uint32_t time);
void newlevel(int level);

void render_horde(void);
void render_turret(void);
void render_buildings(void);
void render_status(void);
void render_gameover(void);

void update_controls(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]);
void update_turret(void);
void update_missile(bool button_p[]);
void update_horde(uint32_t time);
void update_bomb(uint8_t b);
void update_building(void);

void audio(uint32_t time_ms);

};
