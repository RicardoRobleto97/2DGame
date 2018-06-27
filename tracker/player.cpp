#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "collisionStrategy.h"

Player::Player( const std::string& name) :
  MultiSprite(name, 1),
	observers(),
  collision(false),
  initialVelocity(getVelocity()),
	bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
	bulletList(),
	freeList(),
	minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minSpeed")),
	bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
	timeSinceLastFrame(0),
	maxBullets(Gamedata::getInstance().getXmlInt(bulletName+"/max")),
	strategy(new PerPixelCollisionStrategy)
{ }

Player::Player(const Player& s) :
  MultiSprite(s), 
	observers(s.observers),
  collision(s.collision),
  initialVelocity(s.getVelocity()),
	bulletName(s.bulletName),
	bulletList(s.bulletList),
	freeList(s.freeList),
	minSpeed(s.minSpeed),
	bulletInterval(s.bulletInterval),
	timeSinceLastFrame(s.timeSinceLastFrame),
	maxBullets(s.maxBullets),
	strategy(s.strategy)
{ }

Player::~Player() {
	for( auto& bullet : bulletList ) {
    delete bullet;
  }
	for( auto& bullet : freeList ) {
    delete bullet;
  }
	delete strategy;
}

void Player::detach( Drawable *o ) {
	auto it = observers.begin();
	while (it != observers.end()){
		if(*it == o) {
			it == observers.erase(it);
			return;
		}
		++it;
	}
}

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  setVelocityX(0);//0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 

void Player::down() { 
  if ( getY() < worldWidth-getScaledWidth()) {
    setVelocityY(initialVelocity[1]);
  }
} 
void Player::up()  { 
  if ( getY() > 0) {
    setVelocityY(-initialVelocity[1]);
  }
} 

void Player::explode() {
	//subtract life here
	MultiSprite::explode();
}

bool Player::collided(const Drawable *e){
	auto it = bulletList.begin();
	while(it != bulletList.end()) {
		if(strategy->execute(*e, **it)) {
			freeList.emplace_back(*it);
			it = bulletList.erase(it);
			return true;			
		}
		else ++it;
	}
	return false;
}

void Player::shoot() {
	float dX = getScaledWidth()/2;
	float dY = getScaledHeight()/2;

	//limit on bullets
	if(bulletList.size() == maxBullets) return;

	Drawable *bullet;

	//check to see if there are any in the freelist
	if(freeList.size() > 0) {
		bullet = freeList.front();
		freeList.erase(freeList.begin());
	}
	else {
		//else make one
		bullet = new Bullet(bulletName);
	}
	bullet->setPosition(getPosition() + Vector2f(dX-2,-dY+10));
	bullet->setVelocity(Vector2f(0, minSpeed));
	static_cast<Bullet*>(bullet)->reset();
	bulletList.emplace_back(bullet);
}

void Player::draw() const {
	MultiSprite::draw();
	for(auto& bullet : bulletList) {
		bullet->draw();
	}
}

void Player::update(Uint32 ticks) {
  MultiSprite::update(ticks);
	auto it = observers.begin();
	while(it != observers.end()){
		static_cast<SmartSprite*>(*it)->setPlayerPos(getPosition());
		++it;
	}
	auto bullet = bulletList.begin();
	while(bullet != bulletList.end()) {
		(*bullet)->update(ticks);
		if(static_cast<Bullet*>(*bullet)->goneTooFar()) {
			freeList.emplace_back(*bullet);
			bullet = bulletList.erase(bullet);
		}
		else ++bullet;
	}
	
  stop();
}

