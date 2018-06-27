#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartSprite::goLeft()  { setVelocityX( -abs(getVelocityX()) );  }
void SmartSprite::goRight() { setVelocityX( fabs(getVelocityX()) );  }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) ); }


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, 
  int w, int h) :
  TwoWayMultiSprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(SEEK),
  seekDistance(Gamedata::getInstance().getXmlFloat(name+"/seekDistance")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{}


SmartSprite::SmartSprite(const SmartSprite& s) : 
  TwoWayMultiSprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  seekDistance(s.seekDistance),
	worldHeight(s.worldHeight)
{}

void SmartSprite::update(Uint32 ticks) { 
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

	if(distanceToEnemy > seekDistance && y < (worldHeight - 100)) currentMode = SEEK;
	else currentMode = NORMAL;  
	
  if(currentMode == SEEK){
    if ( x < ex ) goRight();
    if ( x > ex ) goLeft();
    if ( y < ey ) goDown();
    if ( y > ey ) goUp();
  }

	TwoWayMultiSprite::update(ticks);
}
