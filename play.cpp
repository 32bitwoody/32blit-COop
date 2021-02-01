#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <ctype.h>

#include "play.hpp"


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

using namespace blit;


play::play(){
	printf("Play Construct\n");
	firstrun=false;
}
//----------------------------------------------------------------------
void play::init(uint8_t &m_gamestate){
	printf("Play Init ");
  screen.sprites = SpriteSheet::load(asset_sprites);
    // Attempt to load the first save slot.
  if(read_save(score_data,0)) {
    // Loaded sucessfully!
  } else {
    // No save file or it failed to load, set up some defaults.
    strncpy(score_data.name, "COop score", 10);
    score_data.score = 0;
    score_data.highscore = 510;
  }
  if(read_save(game_data,1)) {
    gamedataload=true;
    // Loaded sucessfully!
    //reset alien bombs
    for(uint8_t b=0; b<game_data.bombcount; b++) {
      bomb[b]=Point(0,0);
    }
  } else {
    // No save file or it failed to load, set up some defaults.
    strncpy(game_data.name, "COop game", 10);
    game_data.level = 1;
    gamedataload=false;
    game_data.lives=3;
    game_data.turret=80;
    game_data.bombcount=1;

  }
    targetting=150;
    seperate=Point(25,18);
    
    hordemove=Point(20,40);
    direction=Point(1,0);
    hordesprite=0;
    
    hordespeed=100;
  
  gameover=false;
  prevtime=0;
//  score=0;
//  game_data.lives=3;
// half width of target area for horde   
//  targetting=150;
//  seperate=Point(25,18);
  m_gamestate=STATE_PLAY;
  
    channels[3].waveforms   = Waveform::NOISE | Waveform::TRIANGLE;
  channels[3].attack_ms   = 1;
  channels[3].decay_ms    = 64;
  channels[3].sustain     = 0xffff;
  channels[3].release_ms  = 1;
//	printf("-done\n");
}
//----------------------------------------------------------------------
void play::newlevel(int l){
  printf("New level %d\n",l);
  game_data.level=l;
  if(game_data.level==1) {
    score_data.score=0;
    game_data.lives=3;
// half width of target area for horde   
    targetting=150;
//    seperate=Point(25,18);
    game_data.bombcount=0;
  }
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 5; y++) {
//      printf("%d,%d:",x,y);
	    if(y<5) { game_data.horde[x+y*8]=1; }
	    if(y>2) { game_data.horde[x+y*8]=3; }
	    if(y>3) { game_data.horde[x+y*8]=5; }
//      printf("%d ",x+y*8);
//      game_data.building[x+y*8]=1;
      hordetime[x+y*8]=0;
	  }
  }
  hordemove=Point(20,40);
  direction=Point(1,0);
  hordesprite=0;
//  printf("\nBuilding ");
  for (int n = 0; n < 48; n++) {
    game_data.building[n]=1;
//    printf("%d,",game_data.building[n]);
  }
  game_data.turret=80;
  hordespeed=100;
     game_data.bombcount=game_data.bombcount+1;
  if(   game_data.bombcount>10) {
    game_data.bombcount=10;
  }
  for(uint8_t b=0; b<game_data.bombcount; b++) {
    bomb[b]=Point(0,0);
  }
  seperate=seperate+Point(2,2);
//   targetting=targetting-5;
  if(   targetting>6) {
    targetting=targetting-5;
  }
}
//----------------------------------------------------------------------
void play::update(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]){
//  printf("Update. Lives %d\n",game_data.lives);
//  printf("time %d,%d\n",time,time-prevtime);
  update_controls( time,m_gamestate,button_p,  button_r);
//  if(lostlife==true) {
//    if(button_p[ButtonB]==true) {
//      lostlife=false;
//    } else {
//    return;
//    }
//  }
  if(gameover==true && gameack==true) {
    gameover=false;
    gameack=false;
    button_p[ButtonA]=false;
    newlevel(1);
  }
  if(gameover==true && gameack==false) {
//    printf("game over %d ack %d\n",gameover,gameack);
    return;
  }
  if (button_r[ButtonH]==true) {
    button_r[ButtonH]=false;
    m_gamestate = STATE_INITSETTING;
  }
  update_turret();
  update_missile(button_p);
  update_horde(time);

  if(counthorde==0) {
    newlevel(game_data.level+1);
    levelstart=time;
    score_data.score=score_data.score+100;
  }
  update_bomb(0);
  if(game_data.lives==0) {
//    printf(" game over\n");
    gameover=true;  
//    m_gamestate = STATE_SCORES;
  }
  update_building();
  if(score_data.score>score_data.highscore) {
    score_data.highscore = score_data.score;
  }
  audio(time);
}
//----------------------------------------------------------------------
void play::render(uint32_t time){
//  printf("Play: Render\n");
  screen.clear();
  screen.alpha = 255;
  screen.mask = nullptr;

  render_horde();  
  render_turret();
  render_buildings(); 
  render_status() ;
  if(gameover==true) {
    render_gameover();
  }
}
//----------------------------------------------------------------------
void play::render_horde(void) {
//  printf("Play: Display horde\n");
 uint8_t showsprite;
  for(uint8_t b=0; b<game_data.bombcount; b++) {
//    printf("Render bomb %d of %d\n",b,game_data.bombcount);
    screen.sprite(Rect(0,14,1,1), bomb[b]);
  }
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 5; y++) {
//      printf("%d,%d-%d ",x,y,game_data.horde[x+y*8]);
      if (game_data.horde[x+y*8]>0 && game_data.horde[x+y*8]<6) {
        showsprite=hordesprite+x+x;
        if(showsprite>9) { showsprite=showsprite-10; }
//        printf("%d,%d ",seperate.x,seperate.y);
        screen.sprite(Rect(showsprite,game_data.horde[x+y*8]-1,2,2), Point(seperate.x*x, seperate.y*y)+hordemove);
      }
      if (game_data.horde[x+y*8]>5) {
        screen.sprite(Rect(12,18,2,2), Point(seperate.x*x, seperate.y*y)+hordemove);
      }
    }
//    printf("\n");
  }
}
//----------------------------------------------------------------------
void play::render_gameover(void) {
//    printf("Game over panel\n");

  screen.alpha = 255;
  screen.mask = nullptr;
//  screen.pen = Pen(255, 255, 255);
  screen.pen = Pen(58, 94, 66);

//    screen.rectangle(Rect(0, 90, 320, 50));  
  Rect text_rect(5, 95, 310, 40);
  screen.rectangle(text_rect);

  screen.pen = Pen(102, 120, 114);
  std::string text = "Game Over !";
  text = screen.wrap_text(text, text_rect.w, fat_font, true);
  screen.text(text, fat_font, text_rect, true, TextAlign::center_center);

//    screen.text("Game over !", fat_font, Point(5, 95));//, true, TextAlign::center_h);
  text = "Press A to continue";
  text = screen.wrap_text(text, text_rect.w, minimal_font, true);
  screen.text(text, minimal_font, text_rect, true, TextAlign::bottom_center);

//    screen.text("Press A to continue", minimal_font, Point(5, 112));
  screen.pen = Pen(0, 0, 0);

}
//----------------------------------------------------------------------
void play::render_turret(void) {
//  printf("turret m :%d,%d\n",missile.x,missile.y);
  screen.sprite(Rect(12, 16, 1,1), missile);
//  printf("show turret\n");
  screen.sprite(Rect(16, 16, 4, 4), Point(game_data.turret*2-15, 200));
  for(int l=0; l<game_data.lives; l++ ) {
    screen.stretch_blit(screen.sprites, Rect(127,127,32,32), Rect(l*10, 232, 8, 8));
  }

//  screen.sprite(Rect(12,18,2,2), Point(seperate.x*x, seperate.y*y)+hordemove);
//  screen.sprite(Rect(12,18,2,2), Point(seperate.x*x, seperate.y*y)+hordemove);
}
//----------------------------------------------------------------------
void play::render_buildings(void) {
//    printf("show buildings\n");
  uint16_t yy;
  for (int b=0; b<3; b++) {
    for (int y = 16; y < 20; y++) {
      yy=y-16;
      for (int x = 0; x < 4; x++) {
//        printf("piece %d,%d,%d ",b,x,yy);
//        printf("set %d",game_data.building[x+yy*8+b*16]);
        
        if(game_data.building[x+yy*4+b*16]>0) {
          screen.sprite(Rect(x+game_data.building[x+yy*4+b*16]-1, y, 1, 1), Point(x*8+b*106+38, y*8+42));
        }
//   printf("\n");
      }
    }
  }
}
//----------------------------------------------------------------------
void play::render_status(void) {
  screen.pen = Pen(255, 255, 255);
  screen.rectangle(Rect(0, 0, 320, 14));
  screen.pen = Pen(0, 0, 0);
   std::string lv = std::to_string(game_data.level);
  screen.text("LEVEL "+lv, minimal_font, Point(5, 5));
  std::string sc = std::to_string(score_data.score);
  screen.text("SCORE "+sc, minimal_font, Point(110, 5));
  sc = std::to_string(score_data.highscore);
  screen.text("HIGH "+sc, minimal_font, Point(220, 5));
}
//----------------------------------------------------------------------
void play::update_controls(uint32_t time,uint8_t &m_gamestate, bool button_p[], bool button_r[]) {
//  printf("Update Left%d Right%d\n",button_p[DpadLeft],button_p[DpadRight]);
//  printf("Dpad left%d right %d\n",pressed(Button::DPAD_LEFT),pressed(Button::DPAD_RIGHT));
  if(gameover==true && button_p[ButtonA]==true) {
      gameack=true;
      return;
  }
  if(button_p[ButtonA]==true && missile.y==0) {
//    printf("Missile away\n");
    missile.y=210;
    missile.x=game_data.turret*2-2;
  }
  if(button_p[ButtonA]==true && button_p[ButtonB]==true && (time-levelstart)>5000) {
    printf("both A and B pressed\n");
    button_p[ButtonA]==false;
    button_p[ButtonB]==false;
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 5; y++) {
        if(game_data.horde[x+y*8]>0 && game_data.horde[x+y*8]<6) {
          game_data.horde[x+y*8]=6;
          hordetime[x+y*8]=time;
        }
      }
    }
  }
//  if (button_p[ButtonA]==true) {
//	  printf("A pressed\n");
//  }
  if (button_r[ButtonA]==true) {
//    printf("Release: A,Play\n");
    m_gamestate = STATE_PLAY;
  }
  if(button_p[DpadLeft]==true && button_r[DpadLeft]==false)  { 
    game_data.turret=game_data.turret-1;
//    printf("Left pressed\n");
  }
  if(button_p[DpadRight]==true && button_r[DpadRight]==false) {
    game_data.turret=game_data.turret+1;
//    printf("Right pressed\n");
  }
}
//----------------------------------------------------------------------
void play::update_turret(void) {
  
  if(game_data.turret<4) { game_data.turret=4; }
  if(game_data.turret>156) { game_data.turret=156; }
//  printf("Turret:%d\n",game_data.turret);

}
//----------------------------------------------------------------------
void play::update_missile(bool button_p[]) {
//   printf("Update missile\n");

  if(missile.y>0) {
    missile.y=missile.y-1;
  }
  if(missile.y<24) {
    missile.y=0;
  }
}
//----------------------------------------------------------------------
void play::update_bomb(uint8_t t) {
 uint8_t rrh=4;
 uint8_t rch=0;
 uint16_t lowx;
 uint16_t highx;
 uint16_t targetarea;
 uint8_t loopcount;
    printf("bomb count %d\n",game_data.bombcount);

  vibration = 0.0f;
  for(uint8_t b=0; b<game_data.bombcount; b++) {
    printf("bomb %d-%d,%d\n",b,bomb[b].x,bomb[b].y);

  if(bomb[b].y>0) {
    bomb[b].y++;
  }
  if(bomb[b].y==0) {
// horde column determind by turret and current targetting accuracy
    lowx=(game_data.turret*2)-15-targetting;
    if(targetting>game_data.turret) {
      lowx=0;
    }
    highx=(game_data.turret*2)-15+targetting;
    if(highx>319) {
      highx=319;
    }
    targetarea=highx-lowx;
//    printf("Turret %d, %d lowx %d, highx %d ",game_data.turret,targetarea,lowx,highx);
    loopcount=20;
    while(bomb[b].y==0) {
      uint16_t rndx= lowx + std::rand()/((RAND_MAX + 1u)/targetarea); 
//      rch=0;
//      printf("hordemove %d seperate %d ",hordemove.x,seperate.x);
//            printf("randx %d \n",rndx);
      rch =(rndx-hordemove.x)/seperate.x;
       
// random horde column member (0-39)
      rrh=0 + std::rand()/((RAND_MAX + 1u)/4);
//      printf("rch %d, rrh %d \n",rch,rrh);
      if(rrh>7) { rrh=7; }
      if(rch>4) { rch=4; }
      if(game_data.horde[rch+rrh*8]>0 && game_data.horde[rch+rrh*8]<6) {
        bomb[b].x=seperate.x*rch+hordemove.x+4;
        bomb[b].y=seperate.y*rrh+hordemove.y+8;
  // set bomb at member pos 
      }
      loopcount--;
      if(loopcount==0) {
//        printf(" LOOP %d ",loopcount);
        break;
      }
    }
  }
  if(bomb[b].y>232) {
    bomb[b]=Point(0,0);
  }
  if(bomb[b].y>216) { 
    int16_t t=bomb[b].x-game_data.turret*2-15;
//    printf("bomb %d %d height %d turret %d diff %d \n",b,bomb[b].x,bomb[b].y,(game_data.turret*2-15),t);
    if(t>-32 && t<0) {
      game_data.lives--;
      vibration = 0.4f;
      printf("Bang, loose a life\n");
      lostlife=true;
      bomb[b]=Point(0,0);


    }
  }
//      printf("next bomb %d\n",b);

}
//  if(b<game_data.bombcount){
//    printf("next bomb\n");
//    update_bomb(b+1);
//  }
}
//----------------------------------------------------------------------
void play::update_horde(uint32_t time) {
 static bool change_dir=false,change_avl=true,obliterate=false;
 Point pos;
// printf("Update horde c%d a%d\n",change_dir,change_avl);
//  change_dir=false;
//  obliterate=false;
  counthorde=0;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 5; y++) {
      pos.x=seperate.x*x+hordemove.x;
      pos.y=seperate.y*y+hordemove.y;
      if (game_data.horde[x+y*8]==6) {
//        printf("%d,%d is dead\n",x,y);
        if ((time-hordetime[x+y*8])>500) {
//          printf("%d-%d=%d\n",time,hordetime[x+y*8],(time-hordetime[x+y*8]));
          game_data.horde[x+y*8]=0;
          hordetime[x+y*8]=0;
        }
      }
      if (game_data.horde[x+y*8]>0 && game_data.horde[x+y*8]<6) {
        if(missile.x>pos.x && missile.x<pos.x+16 && missile.y>pos.y && missile.y<pos.y+16) {
          game_data.horde[x+y*8]=6;
          hordetime[x+y*8]=time;

          missile=Point(0,0);
          score_data.score=score_data.score+10;
          write_save(score_data,0);
          write_save(game_data,1);  
        }
              
//        printf("%d,%d - %d,%d ",x,y,pos.x,pos.y);
        if((pos.x<0 || pos.x>304) && change_avl==true) {
//          printf("horde%d,%d:at end ",x,y);
          change_dir=true;
          change_avl=false;
        }
      }
      if (game_data.horde[x+y*8]>0 && game_data.horde[x+y*8]<7) {
        counthorde++;
      }
      
    }
  }
//  printf("\n");
  if(change_dir==true) {
    change_dir=false;
    direction.x=direction.x*-1;
    hordemove.y=hordemove.y+1;
    hordespeed=hordespeed-7;
    if(hordespeed<10) { hordespeed=10; }
//    printf("hordespeed:%d \n",hordespeed);
  }
    
  if(time-prevtime>hordespeed) {
//    printf("Move horde\n");
    hordemove=hordemove+direction;
    hordesprite++;
    hordesprite++;
    change_avl=true;

//     printf("horde%d,%d\n",hordemove.x,hordemove.y);

//     channels[0].frequency = 259;
//      channels[0].trigger_attack();
//      channels[0].trigger_release();

    if(hordesprite>9) {
      hordesprite=0;
      tick=0;
//      printf("reset tick ");
    }
    prevtime=time;
  }
}
//----------------------------------------------------------------------
void play::update_building(void) {
  //check each piece of building
  uint16_t yy;
//  printf("Update buildings\n");
  for (int b=0; b<3; b++) {
    for (int y = 16; y < 20; y++) {
      yy=y-16;
      for (int x = 0; x < 4; x++) {
//        printf("piece %d,%d,%d ",b,x,yy);
//        printf("set %d",building[x+yy*8+b*16]);
        
 //       if(building[x+yy*4+b*16]>0) {
//          screen.sprite(Rect(x+building[x+yy*4+b*16]-1, y, 1, 1), Point(x*8+b*106+38, y*8+42));
//        }
//   printf("\n");

        for(uint8_t bb=0; bb<game_data.bombcount; bb++) {
          Point t =Point((bomb[bb].x-(x*8+b*106+38)),(bomb[bb].y-(y*8+42)));
//          printf("building %d-%d,%d:Bomb %d,%d:dif %d,%d ",b,x*8+b*106+38, y*8+42,bomb[a].x,bomb[a].y,t.x,t.y);
          if(t.x>-5 && t.x<9 && t.y>-7 && t.y<0 && game_data.building[x+yy*4+b*16]==9) {
//            printf("bang ");
            game_data.building[x+yy*4+b*16]=0; 
            bomb[bb]=Point(0,0);
          }
          if(t.x>-5 && t.x<9 && t.y>-7 && t.y<0 && game_data.building[x+yy*4+b*16]==5) {
//          printf("bang ");
            game_data.building[x+yy*4+b*16]=9; 
            bomb[bb]=Point(0,0);
          }  
          if(t.x>-5 && t.x<9 && t.y>-7 && t.y<0 && game_data.building[x+yy*4+b*16]==1) {
//          printf("bang ");
            game_data.building[x+yy*4+b*16]=5; 
            bomb[bb]=Point(0,0);
          }  
        } 
      }
    }
//    printf("\n");
  }
}
//----------------------------------------------------------------------
void play::audio(uint32_t time_ms) {
//  printf("audio %d ",time_ms);

//  static uint16_t tick = 0;
  static uint16_t prev_beat = 1;
//    printf("tick:%d prev_beat:%d ",tick,prev_beat);

  beat = (tick / 5) % 10; // 50ms per beat
//    printf("tick:%d beat:%d\n",tick,beat);

  tick++;

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
//  }
//
}
//----------------------------------------------------------------------
