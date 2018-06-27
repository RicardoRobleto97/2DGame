#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "twoWayMultiSprite.h"

class SmartSprite : public TwoWayMultiSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  virtual ~SmartSprite() { } 

  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:
  enum MODE {NORMAL, SEEK};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float seekDistance;
	int worldHeight;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
