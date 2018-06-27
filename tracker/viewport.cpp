#include <sstream>
#include "viewport.h"
#include "ioMod.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0), fps(0), pool1(0), pool2(0),
	HUDx(gdata.getXmlInt("hud/x")),
	HUDy(gdata.getXmlInt("hud/y")),
	HUDw(gdata.getXmlInt("hud/w")),
	HUDh(gdata.getXmlInt("hud/h")),
  objectToTrack(NULL) 
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj;
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {
  IoMod::getInstance().writeText("Tracking: "+objectToTrack->getName(), 30, 30);
	IoMod::getInstance().writeText(gdata.getXmlStr("author"), 20, viewHeight-30, {255,255,255,255});

	std::stringstream ss;
	ss << fps;
	std::string out;
	ss >> out;
	IoMod::getInstance().writeText("FPS: " + out, 30, 60, {175,0,255,255});

	std::stringstream p1;
	p1 << pool1;
	std::string out1;
	p1 >> out1;
	IoMod::getInstance().writeText("Bullet List: " + out1, viewWidth-120, 30, {255,255,255,255});

	std::stringstream p2;
	p2 << pool2;
	std::string out2;
	p2 >> out2;
	IoMod::getInstance().writeText("Free List: " + out2, viewWidth-120, 70, {255,255,255,255});
	
}

void Viewport::drawGameOver() const{
	IoMod::getInstance().writeText("Game Over!", viewWidth/2 - 50, viewHeight/2, {255,255,255,255});
	IoMod::getInstance().writeText("Press R to return to the menu.", viewWidth/2 - 120, viewHeight/2+20, {255,255,255,255});
}

void Viewport::drawWin() const {
	IoMod::getInstance().writeText("You Won!", viewWidth/2 - 50, viewHeight/2, {255,255,255,255});
	IoMod::getInstance().writeText("Press R to return to the menu.", viewWidth/2 - 120, viewHeight/2+20, {255,255,255,255});
}

void Viewport::drawHUD() const {
	IoMod::getInstance().drawHUD(HUDx, HUDy, HUDw, HUDh, {255,255,255,200}, {0,0,0,0});
	IoMod::getInstance().writeText("LEFT: A,  RIGHT: D", HUDx+10, HUDy+10, {0,0,0,255});
	IoMod::getInstance().writeText("SHOOT: SPACE", HUDx+10, HUDy+50, {0,0,0,255});
	IoMod::getInstance().writeText("Don't touch the enemy ships!", HUDx+10, HUDy+100, {0,0,0,255});
}

void Viewport::update(int f, int p1, int p2) {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();
	fps = f;
	pool1 = p1;
	pool2 = p2;

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}

