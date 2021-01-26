/*
 * .cpp -  game for the 32blit 
 *              
 *
 *

 *
 * Coyright (C) 2020   <@.co.uk>
 *
 * This software is provided under the MIT License. See LICENSE.txt for details.
 */

/* System headers. */

#include <cstring>

#include "splash.hpp"
#include "play.hpp"
#include "menu.hpp"
#include "endgame.hpp"
#include "graphics/jpeg.hpp"
#include "assets.hpp"

#include "game.hpp"

using namespace blit;

uint8_t m_gamestate=0;
bool button_p[10] = {0};
bool button_r[10] = {0};
bool setting[7] = {0};

//bool buttonA=false;
bool moveObj=false;
bool buttonB=false;
bool addObj=false;

splash ObjectSplash;
play ObjectGame;
menu ObjectSetting;

void update_controls(void) {
//	printf("%d",button_p[ButtonA]);
  if (pressed(Button::A) && button_p[ButtonA]==false) {
	  button_p[ButtonA]=true;
    button_r[ButtonA]=false;
  }
  if (pressed(Button::B) && button_p[ButtonB]==false) {
	  button_p[ButtonB]=true;
    button_r[ButtonB]=false;
  }
  if(pressed(Button::MENU) && button_p[ButtonM]==false) {
	  button_p[ButtonM]=true;
    button_r[ButtonM]=false;
    printf("Menu Pressed\n");
  }
  if (pressed(Button::HOME) && button_p[ButtonH]==false) {
	  button_p[ButtonH]=true;
    button_r[ButtonH]=false;
                printf("Home Pressed\n");

  }
  if (pressed(Button::X) && button_p[ButtonX]==false) {
	  button_p[ButtonX]=true;
    button_r[ButtonX]=false;
    printf("X Pressed\n");
  }
  if (pressed(Button::Y) && button_p[ButtonY]==false) {
	  button_p[ButtonY]=true;
    button_r[ButtonY]=false;
    printf("Y Pressed\n");
  }

  if (pressed(Button::DPAD_LEFT) && button_p[DpadLeft]==false) {
    button_p[DpadLeft]=true;
    button_r[DpadLeft]=false;
  }
  if (pressed(Button::DPAD_RIGHT) && button_p[DpadRight]==false) {
	  button_p[DpadRight]=true;
    button_r[DpadRight]=false;
  }
  if (pressed(Button::DPAD_UP) && button_p[DpadUp]==false) {
	  button_p[DpadUp]=true;
    button_r[DpadUp]=false;
  }
  if (pressed(Button::DPAD_DOWN) && button_p[DpadDown]==false) {
	  button_p[DpadDown]=true;
    button_r[DpadDown]=false;
  }
  if (pressed(Button::A)==false && button_p[ButtonA]==true) {
    printf("Game Release:A\n");
    button_p[ButtonA]=false;
    button_r[ButtonA]=true;
  }
  if (pressed(Button::B)==false && button_p[ButtonB]==true) {
    printf("Game Release:B\n");
    button_p[ButtonB]=false;
    button_r[ButtonB]=true;
  }
  if (pressed(Button::MENU)==false && button_p[ButtonM]==true) {
	  button_p[ButtonM]=false;
    button_r[ButtonM]=true;
    printf("Menu Release\n");
  }
  if (pressed(Button::HOME)==false && button_p[ButtonH]==true) {
	  button_p[ButtonH]=false;
    button_r[ButtonH]=true;
            printf("Home Release\n");

  }
  if (pressed(Button::X)==false && button_p[ButtonX]==true) {
	  button_p[ButtonX]=false;
    button_r[ButtonX]=true;
    printf("X Release\n");
  }
  if (pressed(Button::Y)==false && button_p[ButtonY]==true) {
	  button_p[ButtonY]=false;
    button_r[ButtonY]=true;
    printf("Y Release\n");
  }
  if (pressed(Button::DPAD_LEFT)==false && button_p[DpadLeft]==true) {
    button_r[DpadLeft]=true;
    button_p[DpadLeft]=false;
  }
  if (pressed(Button::DPAD_RIGHT)==false && button_p[DpadRight]==true) {
    button_r[DpadRight]=true;
    button_p[DpadRight]=false;
  }
   if (pressed(Button::DPAD_UP)==false && button_p[DpadUp]==true) {
    button_r[DpadUp]=true;
    button_p[DpadUp]=false;
  }
  if (pressed(Button::DPAD_DOWN)==false && button_p[DpadDown]==true) {
    button_r[DpadDown]=true;
    button_p[DpadDown]=false;
  }
//  if (joystick.y > -0.2f || joystick.y < 0.2f ) {
//    binky.avl.x=0;
//  }
//  if (joystick.y < -0.2f || joystick.y > 0.2f ) {
//    printf("Joystick y %f",joystick.y);
//	  binky.avl.x=joystick.y/2;
//    printf(",%f",binky.avl.x);
//  }
//  if (joystick.x > -0.2f || joystick.x < 0.2f ) {
//    binky.avl.y=0;
//  }
//  if (joystick.x < -0.2f || joystick.x > 0.2f ) {
//    printf("Joystick x %f",joystick.x);
//	  binky.avl.y=joystick.x/2;
//    printf(",%f",binky.avl.y);
//  }
}

//----------------------------------------------------------------------
// Stubs just to make compile work
//  will be broken off in to their own cpp/hpp
//  or even a class
//

void score_update(void){
}
void score_render(void){
}

///////////////////////////////////////////////////////////////////////////
//
// init()
//
// setup your game here
//
void init() {
  set_screen_mode(ScreenMode::hires);
  m_gamestate = STATE_SPLASH;
  ObjectSplash.init();
  
  setting[0]=true;
  setting[1]=true; 
  setting[2]=true;
  setting[3]=false;
  setting[4]=true;
  setting[5]=false;
  setting[6]=false;

}
//
///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {
// clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
//    screen.clear();
  switch( m_gamestate ) {
  case STATE_SPLASH:      /* Show the user a simple splash screen. */
    ObjectSplash.render(time);
//    ObjectSplash.firstrun=true;
    break;
  case STATE_PLAY:        /* The player is, well, playing! */
//    printf("Game: Render\n");
    ObjectGame.render(time);
    break;
//  case STATE_ENDGAME:      /* The game is done. Save the score. */

//    break;
  case STATE_SCORES:     /* Show the high scores. */
    score_render();
    break;
//  case STATE_INITSETTING:        /* Init game settings. Sound,lights,camera. */
//      ObjectSetting.render(time);
      
//      break;
    case STATE_SETTING:        /* The game settings. Sound,lights,camera. */
//        printf("Menu: Update");
        ObjectSetting.render(time,setting);
//        printf("Menu: Update-Done\n");
      break;
  default:                /* Erk! This should Not Be Possible. */
    break;
  }
}
///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
  // What buttons were pressed.
  update_controls();
    /* What we're updating depends on our state. */
//    printf("Game setting %d\n",setting[0]);
  switch( m_gamestate ) {
    case STATE_SPLASH:      /* Show the user a simple splash screen. */
      ObjectSplash.update(time,m_gamestate,button_p,button_r);
//      if ( m_gamestate == STATE_GAME )
//      {
//        game_init();
//      }
      break;
    case STATE_INITPLAY:        /* The player is, well, playing! */
      ObjectGame.init(m_gamestate);
      if(ObjectGame.gamedataload==false) { // or setting is treu/false
        ObjectGame.newlevel(1);
      }
    break;
    case STATE_PLAY:        /* The player is, well, playing! */
//        printf("Game: Update");

        ObjectGame.update(time,m_gamestate,button_p,button_r);
//        printf("Game: Update-Done\n");

      break;
      
    case STATE_ENDGAME:      /* The game is done. Save the score. */
      endgame_update();
      break;
         case STATE_INITSCORES:     /* Show the high scores. */
      score_update();
      break;
    case STATE_SCORES:     /* Show the high scores. */
      score_update();
      break;
    case STATE_INITSETTING:        /* Init game settings. Sound,lights,camera. */
      ObjectSetting.init(m_gamestate);
      
      break;
    case STATE_SETTING:        /* The game settings. Sound,lights,camera. */
//        printf("Menu: Update");
        ObjectSetting.update(time,m_gamestate,button_p,button_r,setting);
//        printf("Menu: Update-Done\n");
      break;
    default:                /* Ahh! This should Not Be Possible. */
      break;
//    printf("Game post setting %d\n",setting[0]);

  }
}

