#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <list>
#include <cmath>
#include "multisprite.h"
#include "bullet.h"

class SmartSprite;
class CollisionStrategy;

class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
	~Player();
	virtual void draw() const;
  virtual void update(Uint32 ticks);

	virtual void explode();
	void shoot();

	void attach(Drawable* o) { observers.emplace_back(o); }
	void detach(Drawable* o);

  bool collided(const Drawable *);
  void missed() { collision = false; }
  Player& operator=(const Player&) = delete;

	//std::list<Drawable*>* getBullets() { return &bulletList; }
	unsigned int getBulletListSize() const { return bulletList.size(); }
	unsigned int getFreeListSize() const { return freeList.size(); }

  void right();
  void left();
	void up();
	void down();
  void stop();
private:
	std::list<Drawable *> observers;
  bool collision;
  Vector2f initialVelocity;
	std::string bulletName;
	std::list<Drawable *> bulletList;
	std::list<Drawable *> freeList;
	float minSpeed;
	float bulletInterval;
	float timeSinceLastFrame;
	unsigned int maxBullets;
	CollisionStrategy* strategy;
};
#endif
