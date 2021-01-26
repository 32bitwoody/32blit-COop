#include <string>
#include <cstring>
#include <memory>
#include <cstdlib>
#include <ctype.h>

#include "endgame.hpp"

using namespace blit;

 void endgame_update(void){
}
void endgame_render(void){
  // draw some text at the top of the screen
    screen.alpha = 255;
    screen.mask = nullptr;
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(0, 0, 320, 14));
    screen.pen = Pen(0, 0, 0);
    screen.text("Game over 32blit!", minimal_font, Point(5, 4));
}
