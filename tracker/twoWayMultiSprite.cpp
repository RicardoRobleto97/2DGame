#include "twoWayMultiSprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f TwoWayMultiSprite::offset(int x, int y, int o) const {
	float a = Gamedata::getInstance().getRandInRange(x-o,x+o);
	float b = Gamedata::getInstance().getRandInRange(y-o,y+o);
	if(rand() % 2) a = a * -1;
	if(rand() % 2) b = b * -1;
	return Vector2f(a,b);
}

TwoWayMultiSprite::~TwoWayMultiSprite() { if (explosion) delete explosion; }

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           offset(
										Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"),
										Gamedata::getInstance().getXmlInt(name+"/offset"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
	explosion(NULL),
	exploded(false),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	flipType(SDL_FLIP_NONE)
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  Drawable(s), 
  images(s.images),
	explosion(s.explosion),
	exploded(s.exploded),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
	flipType(s.flipType)
  { }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
	explosion = (s.explosion);
	exploded = (s.exploded);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
	flipType = (s.flipType);
  return *this;
}

void TwoWayMultiSprite::explode() {
	if (!explosion) {
		Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
		explosion = new ExplodingSprite(sprite);
	}
}

void TwoWayMultiSprite::draw() const { 
	if (explosion) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale(), 0.0, NULL, flipType);
}

void TwoWayMultiSprite::update(Uint32 ticks) { 
	if(explosion) {
		explosion->update(ticks);
		if(explosion->chunkCount() == 0) {
			exploded = true;
			delete explosion;
			explosion = NULL;
		}
		return;
	}

  if(frameInterval != 0)
		advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
		setY(0.0);
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

	if (getVelocityY() > 0 ) flipType = SDL_FLIP_VERTICAL;
	else flipType = SDL_FLIP_NONE;
}
